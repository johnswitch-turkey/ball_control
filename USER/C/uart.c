#include "uart.h"

#include "stm32f4xx_hal.h"

#include "stdio.h"
#include "usart.h"



int GetOpenmvDataCount  = 0;
uint8_t  Cx=0,Cy=0,Finded_flag=0,Ch=0;

extern uint8_t uart1_rxbuff;


void   Openmv_Receive_Data(uint8_t com_data)
{
  
    uint8_t i;
		static uint8_t RxCounter1=0;//计数
		static uint16_t RxBuffer1[10]={0};
		static uint8_t RxState = 0;	
		static uint8_t RxFlag1 = 0;

  if(RxState==0&&com_data==0x2C)  //0x2c帧头
				{
          
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;   //一般情况下，RxBuffer1[0] 存放“<bbhhhhhb”,然后RxBuffer1[1]放的就是0x0c
				}

				else if(RxState==1&&com_data==0x12)  //0x12帧头
				{
					HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
				else if(RxState==2)
				{
           
					RxBuffer1[RxCounter1++]=com_data;
					if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
					{
						RxState=3;
						RxFlag1=1;
            
						//不用每一步都想自己在脑子里复现过程，没那个时间跟精力，对比调试对了就行
						 //正常情况下,运行到这RxCounter1 == 7？  7-5 = 2  
						Cx=RxBuffer1[RxCounter1-5]; 
						Cy=RxBuffer1[RxCounter1-4];
						Finded_flag=RxBuffer1[RxCounter1-3];
						Ch=RxBuffer1[RxCounter1-2];
						GetOpenmvDataCount++;
						
//            printf("%d   ",Cx);
//            printf("%d   ",Cy);
//            printf("%d   ",Finded_flag);
//            printf("%d\n",Ch); 
//               if(RxState==1)
//          {
//            HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
//          }
//          else if(RxState!=1&&RxState!=0)
//          {
//            HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
//          }
					}
			}
		
				else if(RxState==3)		//检测是否接受到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
								
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //将存放数据数组清零
									}
						}
				} 
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //将存放数据数组清零
						}
				}
      }

/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
// int fputc(int ch, FILE *f)
// {
//   HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//   return ch;
// }
 
/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
// int fgetc(FILE *f)
// {
//   uint8_t ch = 0;
//   HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
//   return ch;
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  uint8_t tem;// 这里的是无符号的
	
  if(huart->Instance==USART1)
  {
    tem=uart1_rxbuff;
    Openmv_Receive_Data(tem);
  }	
   HAL_UART_Receive_IT(&huart1,&uart1_rxbuff,1);  //没看懂参数二里的东西
	
}

