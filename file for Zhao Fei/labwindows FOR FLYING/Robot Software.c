#include <analysis.h>
#include "toolbox.h"
#include <ansi_c.h>
#include <formatio.h>
#include <utility.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include "e:\[001]2018��ҵ�������\001�����ļ�����\002-��λ���㷨���\2018711��λ����labwindows��\Robot Software.h"
#include <ansi_c.h>


#define _FORMAT_16_	    16
#define _FORMAT_CHAR_	8
char DataGet[256];
/******************************************************************************************
��ע��ע����Ӣ������
      CPG�����Һ������⺯���������Һ�����������ƫ�ñ���
******************************************************************************************/
int CVICALLBACK uartRevData (int protNo, int event,void *callbackData);
void DisplayRS232Error (int RS232Error);

static int panelHandle;

typedef struct{
		int switch_Value;//open/close uart value  
		int comValue;	
		int bps;
		char check;//У��λ
		char data; //����λ
		char stop; //ֹͣλ
		char revData[512];
		char sendData[256];
		char choice_Format;
		unsigned int  rev_StrLen_Keep;//�����ַ�����
	}strUart;
 strUart Uart;
	
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Robot Software.uir", PANEL)) < 0)
		return -1;
	Uart.choice_Format=_FORMAT_16_;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK Panel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:

			break;
	}
	return 0;
}
/********************************************************************
>>>check �� butCheckX 16����   butCheckI 10����
**********************************************************************/
int CVICALLBACK butCheckX (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (panelHandle, PANEL_CHECKBOX, 1);
			SetCtrlVal (panelHandle, PANEL_CHECKBOX_2, 0);
			Uart.choice_Format=_FORMAT_16_;
			break;
	}
	return 0;
}

int CVICALLBACK butCheckI (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (panelHandle, PANEL_CHECKBOX, 0);
			SetCtrlVal (panelHandle, PANEL_CHECKBOX_2, 1);
			Uart.choice_Format=_FORMAT_CHAR_;
			break;
	}
	return 0;
}
/********************************************************************
>>>button ��
**********************************************************************/
int CVICALLBACK buttonHelp (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//������Ϣ�Ի���
			MessagePopup ("�������պ����ѧ", "�����ࣺ����A314");
			break;
	}
	return 0;
}

int CVICALLBACK buttonQuit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//�˳��û�����
			QuitUserInterface (0);
			break;
	}
	return 0;
}



int CVICALLBACK buttonClearCount (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���÷��ͽ��ռ���ֵΪ0
			SetCtrlVal (panelHandle, PANEL_TEXTMSG_4, "0");
			SetCtrlVal (panelHandle, PANEL_TEXTMSG_2, "0");
			Uart.rev_StrLen_Keep=0; 
			//�����ǰcom����input���������ַ�
			FlushInQ (Uart.comValue);
			//�����ǰcom����output���������ַ�
			FlushOutQ (Uart.comValue);
			break;
	}
	return 0;
}

int CVICALLBACK buttonClearRev (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���ַ������ı������ݣ�������ı�������
			ResetTextBox (panelHandle, PANEL_TEXTBOX, "");
			SetCtrlVal (panelHandle, PANEL_A1, 30); 
			SetCtrlVal (panelHandle, PANEL_A2, 0);
			SetCtrlVal (panelHandle, PANEL_A3, 30);
			SetCtrlVal (panelHandle, PANEL_A4, 0); 
			SetCtrlVal (panelHandle, PANEL_A5, 0);
			SetCtrlVal (panelHandle, PANEL_A6, 0);
			break;
	}
	return 0;
}

int CVICALLBACK buttonClearSend (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//��շ������ַ���
			SetCtrlVal (panelHandle, PANEL_A1, 0); 
			SetCtrlVal (panelHandle, PANEL_A2, 0);
			SetCtrlVal (panelHandle, PANEL_A3, 0);
			SetCtrlVal (panelHandle, PANEL_A4, 0); 
			SetCtrlVal (panelHandle, PANEL_A5, 0);
			SetCtrlVal (panelHandle, PANEL_A6, 0);
			break;
	}
	return 0;
}

int CVICALLBACK buttonSend (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			//ȡ�÷��������ַ���
		   	GetCtrlVal(PANEL,PANEL_A1,&DataGet[1]);		 //���λ�õĶ���ֵ
			GetCtrlVal(PANEL,PANEL_A2,&DataGet[2]);
			GetCtrlVal(PANEL,PANEL_A3,&DataGet[3]);
			GetCtrlVal(PANEL,PANEL_A4,&DataGet[4]);    
			GetCtrlVal(PANEL,PANEL_A5,&DataGet[5]);
			GetCtrlVal(PANEL,PANEL_A6,&DataGet[6]);

			Uart.sendData[1]=DataGet[1];  //���1λ�õı궨
			Uart.sendData[2]=DataGet[2]; //���2λ�õı궨
			Uart.sendData[3]=DataGet[3];   //���3λ�õı궨
		    Uart.sendData[4]=DataGet[4]+10;  //ƫ��ϵ�� 
			//���5λ�õı궨
			Uart.sendData[5]=DataGet[5]+50;   
			Uart.sendData[6]=-DataGet[6]+50;
			// GetCtrlVal (panelHandle,PANEL_STRING_2 , Uart.sendData);
			 //������������ַ���
			 FlushOutQ (Uart.comValue); 
			 for(i=1;i<7;i++)
			 {
				//���ַ���д�����COM����	   
			 ComWrtByte (Uart.comValue,Uart.sendData[i]);
			}
			break;
	}
	return 0;
}
/********************************************************************
>>>switch ��
**********************************************************************/
/********************************************************************
>>>function:switchOpenCom �򿪴���
>>>input:event �¼�����

>>>output:
>>>
**********************************************************************/

int CVICALLBACK switchOpenCom (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int flag_com=0;
	char ErrorMessage[200];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_BINARYSWITCH, &Uart.switch_Value);
			if(Uart.switch_Value){ //open COM
				GetCtrlVal (panelHandle, PANEL_RING, &Uart.comValue);
				GetCtrlVal (panelHandle, PANEL_RING_2, &Uart.bps);
				GetCtrlVal (panelHandle, PANEL_RING_3, &Uart.check);
				GetCtrlVal (panelHandle, PANEL_RING_4, &Uart.data);
				GetCtrlVal (panelHandle, PANEL_RING_5, &Uart.stop);
				//�����ܴ��󱨸�
				DisableBreakOnLibraryErrors ();
				//�򿪲����ô��ڲ���
				flag_com=OpenComConfig (Uart.comValue, "",Uart.bps, Uart.check, Uart.data,Uart.stop, 64, 512);
				//ʹ�ܴ��󱨸�
				EnableBreakOnLibraryErrors ();
				if(!flag_com){
					//Delay (1);
					//�򿪴���ʱLED����
					SetCtrlVal (panelHandle, PANEL_LED, 1);
					FlushInQ (Uart.comValue);
				 	FlushOutQ(Uart.comValue);								   
					SetComTime (Uart.comValue, 1);//������ʱ��
					//����Ϊĳ���ض���COM�ڰ�װһ��ͬ���ص�����
					InstallComCallback (Uart.comValue, LWRS_RXCHAR, 0, 0, (void *)uartRevData, 0);
				}
				else{
					//�����������
					Uart.rev_StrLen_Keep=0;
					SetCtrlVal (panelHandle, PANEL_BINARYSWITCH, 0);
					DisplayRS232Error(flag_com);
				}
			}
			else{
				SetCtrlVal (panelHandle, PANEL_LED, 0);
				DisableBreakOnLibraryErrors ();
				CloseCom (Uart.comValue);
			}
			break;
	}
	return 0;
}
/********************************************************************
>>>function:uartRevData ���ڽ���message���Ҵ���
>>>input:Uart.switch_Value�����ڴ򿪳ɹ���

>>>output:Uart.revData Uart.rev_StrLen_Keep(����)
		  Uart.admit   
>>>
**********************************************************************/

int CVICALLBACK uartRevData (int protNo, int event,void *callbackData)
{
	unsigned int i;
	unsigned int rev_StrLen;//�������ݳ���  
	char rev_StrLen1[8];    //���ճ���ת��
	char rev_Data[512];		//��ʱ���ݽ���
	char rev_Data1[512];		//��ʱ���ݽ��� 
	char  rev_Ache[4]; 		//16������ʾ
	unsigned int set_Length=20;
	
	if(event& LWRS_RXCHAR){
		Fmt(rev_Data, "%s", "");//ת����ʽ
		Fmt(Uart.revData, "%s", "");
		Delay(0.5);
		//��ȡ��ǰ���뻺�����ַ�����
		rev_StrLen=GetInQLen (Uart.comValue);
		Uart.rev_StrLen_Keep+=rev_StrLen; //****ͳ�ƽ����ַ� 
		Fmt(rev_StrLen1, "%s<%i", Uart.rev_StrLen_Keep);
		SetCtrlVal (panelHandle, PANEL_TEXTMSG_4, rev_StrLen1);
		GetCtrlVal (panelHandle, PANEL_RING_6, &set_Length);
		
		
		if(rev_StrLen>0){
			if(Uart.choice_Format==_FORMAT_CHAR_){//�ַ�
				for(i=0;i<rev_StrLen;i++){			//�����ֽڶ�  
					 rev_Data[i]=ComRdByte(Uart.comValue);
					 Uart.revData[i]=rev_Data[i];
				}
				Uart.revData[i]='\0';
				SetCtrlVal (panelHandle, PANEL_TEXTBOX, Uart.revData);
				rev_StrLen=0;
				FlushInQ(Uart.comValue);
			}
			else if(Uart.choice_Format==_FORMAT_16_){//16����
				for(i=0;i<rev_StrLen;i++){			//�����ֽڶ�  
					 rev_Data[i]=ComRdByte(Uart.comValue);
					 Uart.revData[i]=rev_Data[i];	//ȥ��ֹͣλ
					 switch(0xf0&rev_Data[i]){
					 	case 0x00:
							rev_Data1[i]= '0';
							break;
						case 0x10:
							rev_Data1[i]= '1';
							break;
						case 0x20:
							rev_Data1[i]= '2';
							break;
						case 0x30:
							rev_Data1[i]= '3';
							break;
						case 0x40:
							rev_Data1[i]= '4';
							break;
						case 0x50:
							rev_Data1[i]= '5';
							break;
						case 0x60:
							rev_Data1[i]= '6';
							break;
						case 0x70:
							rev_Data1[i]= '7';
							break;
						case 0x80:
							rev_Data1[i]= '8';
							break;
						case 0x90:
							rev_Data1[i]= '9';
							break;
						case 0xa0:
							rev_Data1[i]= 'a';
							break;
						case 0xb0:
							rev_Data1[i]= 'b';
							break;
						case 0xc0:
							rev_Data1[i]= 'c';
							break;
						case 0xd0:
							rev_Data1[i]= 'd';
							break;
						case 0xe0:
							rev_Data1[i]= 'e';
							break;
						case 0xf0:
							rev_Data1[i]= 'f';
							break;
						default: break;
					 }
					 switch(0x0f&rev_Data[i]){
					 	case 0x00:
							rev_Data1[i+1]= '0';
							break;
						case 0x01:
							rev_Data1[i+1]= '1';
							break;
						case 0x02:
							rev_Data1[i+1]= '2';
							break;
						case 0x03:
							rev_Data1[i+1]= '3';
							break;
						case 0x04:
							rev_Data1[i+1]= '4';
							break;
						case 0x05:
							rev_Data1[i+1]= '5';
							break;
						case 0x06:
							rev_Data1[i+1]= '6';
							break;
						case 0x07:
							rev_Data1[i+1]= '7';
							break;
						case 0x08:
							rev_Data1[i+1]= '8';
							break;
						case 0x09:
							rev_Data1[i+1]= '9';
							break;
						case 0x0a:
							rev_Data1[i+1]= 'a';
							break;
						case 0x0b:
							rev_Data1[i+1]= 'b';
							break;
						case 0x0c:
							rev_Data1[i+1]= 'c';
							break;
						case 0x0d:
							rev_Data1[i+1]= 'd';
							break;
						case 0x0e:
							rev_Data1[i+1]= 'e';
							break;
						case 0x0f:
							rev_Data1[i+1]= 'f';
							break;
						default: break;
					 }
					 rev_Ache[0]=rev_Data1[i];
					 rev_Ache[1]=rev_Data1[i+1]; 
					 rev_Ache[2]='\0';
					 SetCtrlVal (panelHandle, PANEL_TEXTBOX, rev_Ache);
					 SetCtrlVal (panelHandle, PANEL_TEXTBOX, " ");
					 if(i==(set_Length-1)){
					 	SetCtrlVal (panelHandle, PANEL_TEXTBOX, "\n");
					 }
					 
				}
				rev_StrLen=0;
				FlushInQ(Uart.comValue);
			}
		}
	}
	return 0; 
}

//**************************************************************************//
//		��������:	DisplayRS232Error										//
//										//
//		��������:															//
//					��ʾRS232������Ϣ										//
//**************************************************************************//
void DisplayRS232Error (int RS232Error)
{
    char ErrorMessage[200];
    
    switch (RS232Error)
        {
       case 0  :
            MessagePopup ("RS232 Message", "No errors.");
            break;
            
        case -2 :
            Fmt (ErrorMessage, "%s", "Invalid port number (must be in the "
                                     "range 1 to 8).");
            MessagePopup ("RS232 Message", ErrorMessage);
            break;
            
        case -3 :
            Fmt (ErrorMessage, "%s", "No port is open.\n"
                 "Check COM Port setting in Configure.");
            MessagePopup ("RS232 Message", ErrorMessage);
            break;
            
        case -6 :
            Fmt (ErrorMessage, "%s", "No serial port found..." );
            MessagePopup ("RS232 Message", ErrorMessage);
            break;
            
        case -7 :
            Fmt (ErrorMessage, "%s", "Can not open port\n"
                 "Please check if it is in use.");
            MessagePopup ("RS232 Message", ErrorMessage);
            break;
            
        case -99 :
            Fmt (ErrorMessage, "%s", "Timeout error.\n\n"
                 "Either increase timeout value,\n"
                 "       check COM Port setting, or\n"
                 "       check device.");
            MessagePopup ("RS232 Message", ErrorMessage);
            break;
            
        default :
            if (RS232Error < 0)
            {  
                Fmt (ErrorMessage, "%s<RS232 error number %i", RS232Error);
                MessagePopup ("RS232 Message", ErrorMessage);
            }
            break;
            
        }
}




