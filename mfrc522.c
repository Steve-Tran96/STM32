#include "MFRC522.h"
#include "gpio.h"
/*
 * Chuc nang: Viet 1 byte du lieu vao thanh ghi MFRC522
 */
uint8_t RC522_SPI_Transfer(MFRC522_Handled *xpSPI, uint8_t data){
	
	while((SPI_I2S_GetFlagStatus(xpSPI, (uint16_t)0x0002)) == RESET);
	SPI_I2S_SendData(xpSPI->xSPI_RFC522, data);
	
	while((SPI_I2S_GetFlagStatus(xpSPI, (uint16_t)0x0001)) == RESET); 
	return (uint8_t)SPI_I2S_ReceiveData(xpSPI->xSPI_RFC522);
}

void Write_MFRC522(MFRC522_Handled *xCSPin, uint8_t addr, uint8_t val)
{
	/* CS LOW */
	GPIO_Write(xCSPin->pGPIOFRC522, RESET, xCSPin->MFRC522_Pin);

	//DFF (Write) of RFC522 :0XXXXXX0
	RC522_SPI_Transfer(xCSPin, (addr<<1)&0x7E);	
	RC522_SPI_Transfer(xCSPin, val);
	
	/* CS HIGH */
	GPIO_Write(xCSPin->pGPIOFRC522, SET, xCSPin->MFRC522_Pin);
}

uint8_t Read_MFRC522(MFRC522_Handled *xCSPin, uint8_t addr){
	uint8_t val;

	/* CS LOW */
	GPIO_Write(xCSPin->pGPIOFRC522, RESET, xCSPin->MFRC522_Pin);

	//Dinh dang dia chi:1XXXXXX0
	RC522_SPI_Transfer(xCSPin,((addr<<1)&0x7E) | 0x80);	
	val = RC522_SPI_Transfer(xCSPin,0x00);
	
	/* CS HIGH */
	GPIO_Write(xCSPin->pGPIOFRC522, SET, xCSPin->MFRC522_Pin);
	
	return val;		
}
/*==============================================================*/

void SetBitMask(MFRC522_Handled *xCSPin, uint8_t reg, uint8_t mask) {
    uint8_t tmp;
    tmp = Read_MFRC522(xCSPin, reg);
    Write_MFRC522(xCSPin, reg, (tmp | mask));  // set bit mask
}

void ClearBitMask(MFRC522_Handled *xCSPin, uint8_t reg, uint8_t mask)  {
    uint8_t tmp;
    tmp = Read_MFRC522(xCSPin, reg);
    Write_MFRC522(xCSPin, reg,( tmp & (~mask)));  // clear bit mask
} 

void AntennaOn(MFRC522_Handled *xCSPin)
{
	uint8_t temp;

	temp = Read_MFRC522(xCSPin, TxControlReg);
//	if (!(temp & 0x03))
//	{
//		SetBitMask(xCSPin, TxControlReg, 0x03);
//	}
	SetBitMask(xCSPin,TxControlReg, 0x03);
}

void MFRC522_Reset(MFRC522_Handled *xCSPin){
    Write_MFRC522(xCSPin, CommandReg, PCD_RESETPHASE);
}

/* SPI configuration with:
Prescaler: /8
Bus: Full Duplex
Mode: Master
Datsize: 8 bits
CPOL: LOW, CPHA: 1 Edge
NSS: Software
Firstbit: MSB
CRCPR = 0x7h
*/

void MFRC522_Init(MFRC522_Handled *xCSPin){
	MFRC522_Reset(xCSPin);
	 	
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
	Write_MFRC522(xCSPin, TModeReg, 0x8D);		//Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	Write_MFRC522(xCSPin, TPrescalerReg, 0x3E);	//TModeReg[3..0] + TPrescalerReg
	Write_MFRC522(xCSPin, TReloadRegL, 30);           
	Write_MFRC522(xCSPin, TReloadRegH, 0);
	
	Write_MFRC522(xCSPin, TxAutoReg, 0x40);		//100%ASK
	Write_MFRC522(xCSPin, ModeReg, 0x3D);		//CRC Gia tri ban dau 0x6363	???

	//ClearBitMask(Status2Reg, 0x08);		//MFCrypto1On=0
	//Write_MFRC522(RxSelReg, 0x86);		//RxWait = RxSelReg[5..0]
	//Write_MFRC522(RFCfgReg, 0x7F);   		//RxGain = 48dB

	AntennaOn(xCSPin);		//Mo Anten
}

uint8_t MFRC522_ToCard(MFRC522_Handled *xCSPin, uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint32_t *backLen){
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;

    switch (command)
    {
        case PCD_AUTHENT:		//Xac nhan the gan
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:	// Gui du lieu FIFO
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
    }
   
    Write_MFRC522(xCSPin,CommIEnReg, irqEn|0x80);	//Yeu cau ngat
    ClearBitMask(xCSPin,CommIrqReg, 0x80);			//Clear tat ca cac bit yeu cau ngat
    SetBitMask(xCSPin,FIFOLevelReg, 0x80);			//FlushBuffer=1, Khoi tao FIFO
    
	Write_MFRC522(xCSPin,CommandReg, PCD_IDLE);	//NO action; Huy bo lenh hien hanh	???

	// Ghi du lieu vao FIFO
    for (i=0; i<sendLen; i++)
    {   
		Write_MFRC522(xCSPin,FIFODataReg, sendData[i]);    
	}

	//chay
	Write_MFRC522(xCSPin,CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {    
		SetBitMask(xCSPin,BitFramingReg, 0x80);		//StartSend=1,transmission of data starts  
	}   
    
	//Cho doi de nhan duoc du lieu day du
	i = 2000;	//i tuy thuoc tan so thach anh, thoi gian toi da cho the M1 la 25ms
    do 
    {
		//CommIrqReg[7..0]
		//Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = Read_MFRC522(xCSPin,CommIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitIRq));

    ClearBitMask(xCSPin,BitFramingReg, 0x80);			//StartSend=0
	
    if (i != 0)
    {    
        if(!(Read_MFRC522(xCSPin,ErrorReg) & 0x1B))	//BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   
							status = MI_NOTAGERR;			//??   
						}

            if (command == PCD_TRANSCEIVE)
            {
               	n = Read_MFRC522(xCSPin,FIFOLevelReg);
              	lastBits = Read_MFRC522(xCSPin,ControlReg) & 0x07;
                if (lastBits)
                {   
									*backLen = (n-1)*8 + lastBits;   
								}
												else
												{   
									*backLen = n*8;   
								}

												if (n == 0)
												{   
									n = 1;    
								}
												if (n > MAX_LEN_MFRC)
												{   
									n = MAX_LEN_MFRC;   
								}
								
								//Doc FIFO trong cac du lieu nhan duoc
												for (i=0; i<n; i++)
												{   
									backData[i] = Read_MFRC522(xCSPin, FIFODataReg);    
								}
            }
        }
        else
        {   
			status = MI_ERR;  
		}
  }
    //SetBitMask(ControlReg,0x80);           //timer stops
    //Write_MFRC522(CommandReg, PCD_IDLE); 
    return status;
}

/*
 * Chuc nang:Phat hien the, doc loai the
 * Input:reqMode--Phat hien co the,
 *			 TagType--Loai the tra ve
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 * Return: MI_OK neu thanh cong
 */
uint8_t MFRC522_Request(MFRC522_Handled *xCSPin, uint8_t reqMode, uint8_t *TagType){
	uint8_t status;  
	uint32_t backBits;			//cac bit du lieu nhan duoc

	Write_MFRC522(xCSPin, BitFramingReg, 0x07);		//TxLastBists = BitFramingReg[2..0]	???
	
	TagType[0] = reqMode;
	status = MFRC522_ToCard(xCSPin, PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

	if ((status != MI_OK) || (backBits != 0x10))
	{    
		status = MI_ERR;
	}
   
	return status;
}

uint8_t MFRC522_Anticoll(MFRC522_Handled *xCSPin, uint8_t *serNum){
  uint8_t status;
  uint8_t i;
	uint8_t serNumCheck=0;
  uint32_t unLen;
    
    //ClearBitMask(Status2Reg, 0x08);		//TempSensclear
    //ClearBitMask(CollReg,0x80);			//ValuesAfterColl
	Write_MFRC522(xCSPin, BitFramingReg, 0x00);		//TxLastBists = BitFramingReg[2..0]
  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = MFRC522_ToCard(xCSPin, PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

  if (status == MI_OK)
	{
		//Kiem tra so serial the
		for (i=0; i<4; i++)
		{   
		 	serNumCheck ^= serNum[i];
		}
		if (serNumCheck != serNum[i])
		{   
			status = MI_ERR;    
		}
  }
    //SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1
  return status;
}

FlagStatus SPI_I2S_GetFlagStatus(MFRC522_Handled* SPIx, uint16_t SPI_I2S_FLAG){
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  /* Check the status of the specified SPI/I2S flag */
  if ((SPIx->xSPI_RFC522->SR & SPI_I2S_FLAG) != (uint16_t)RESET)
  {
    /* SPI_I2S_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* SPI_I2S_FLAG is reset */
    bitstatus = RESET;
  }
  /* Return the SPI_I2S_FLAG status */
  return  bitstatus;
}
