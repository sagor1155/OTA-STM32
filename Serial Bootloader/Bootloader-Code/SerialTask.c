
#include "SerialTask.h"



static char rx_index, rx_data[2], rx_buffer[100], transfer_complete;
static uint8_t data_length = 0;
static bool packet_start = false;

UART_HandleTypeDef huart1;


void serial_begin(void)
{
  huart1.Instance 				= USART1;
  huart1.Init.BaudRate 		= 9600;
  huart1.Init.WordLength 	= UART_WORDLENGTH_8B;
  huart1.Init.StopBits 		= UART_STOPBITS_1;
  huart1.Init.Parity 			= UART_PARITY_NONE;
  huart1.Init.Mode 				= UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
  huart1.Init.OverSampling 		= UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling 	= UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit 			= UART_ADVFEATURE_DMADISABLEONERROR_INIT|UART_ADVFEATURE_MSBFIRST_INIT;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  huart1.AdvancedInit.MSBFirst = UART_ADVFEATURE_MSBFIRST_DISABLE;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

	HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_data, 1);   //activate UART receive 
	serial_flush_buffer();
}


void serial_print(const char* ptr)
{
	uint8_t pBuff;
	while((*ptr) != '\0'){
		pBuff = (uint8_t)(*(ptr++));
		HAL_UART_Transmit(&huart1, &pBuff, 1, 10);
	}		
}


void serial_println(const char* ptr)
{
	serial_print(ptr);
	serial_print("\n");
}


bool serial_is_hex_received(void)
{
	return (transfer_complete == 1) ? true : false;
}


void serial_clear_hex_receive_flag(void)
{
	transfer_complete = 0;
}


uint8_t serial_get_hex_string_length(void)
{
	return data_length;
}

void serial_flush_buffer(void)
{
	memset(rx_buffer, '\0', sizeof(rx_buffer));
}


void serial_print_u32(uint32_t data)
{
	char str[20];
	sprintf(str, "%x", data);
	serial_print(str);
}

void serial_println_u32(uint32_t data)
{
	serial_print_u32(data);
	serial_print("\n");
}


static uint8_t hex2int(char hex) {
  uint8_t byte = hex; 
  // transform hex character to the 4bit equivalent number, using the ascii table indexes
  if (byte >= '0' && byte <= '9') byte = byte - '0';
  else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
  else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
  // shift 4 to make space for new digit, and add the 4 bits of the new digit 
  return byte;
}

static uint32_t merge_digit(uint8_t* ptr_buffer, uint8_t size)
{
  uint32_t value = 0;  
  for(int i=0; i<size; i++){
    value = value | (ptr_buffer[i] << (size - 1 - i)*4);
  }
  return value;
}

static void merge_digit_buffer(uint8_t* data_buffer, uint8_t* input_buffer, uint8_t number_0f_bytes)
{
  for(int i=0; i<number_0f_bytes; i++){
      data_buffer[i] = merge_digit(&input_buffer[i*2], 2);
  }
}


void serial_process_hex_buffer(uint8_t* output_buffer)
{
	uint8_t data_buffer[50];
	//char str[50];
	
	for(int i=0; i<data_length; i++)	
	{
		data_buffer[i] = hex2int(rx_buffer[i]);
	}			
	merge_digit_buffer(output_buffer, data_buffer, (uint8_t)(data_length/2));

//			for(int i=0; i<serial_get_hex_string_length()/2; i++)
//			{
//				memset(str, '\0', sizeof(str));
//				sprintf(str, "%u", output_buffer[i]);
//				serial_print(str);
//				serial_print(",");
//			}
	
	//serial_print("\n");
}


//Interrupt callback routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  
{
  if (huart->Instance == USART1)  //current UART
  {		
		if ((char)rx_data[0] == ':')
		{
		  serial_flush_buffer(); 
			data_length = 0;
			transfer_complete = 0;
      packet_start = true;	
		}
		else if ((char)rx_data[0] == '\n')                     
		{
			data_length = rx_index;
			rx_index = 0;
			transfer_complete = 1;                  
			packet_start = false;
		}
		else                                       
		{
			rx_buffer[rx_index++] = rx_data[0];
		}		


/*		
		if (rx_index==0)
		{
			serial_flush_buffer(); 
		}
		
		if ((char)rx_data[0] == ':')
		{
			//hex string packet started
      serial_flush_buffer(); 
			data_length = 0;
			transfer_complete = 0;
      packet_start = true;
   			
		}
		
		//and check packet start
		else if (((char)rx_data[0] != '\n') && packet_start==true)              //if received data different from ascii 13 (enter)       
		{
			rx_buffer[rx_index++] = rx_data[0];     //add data to rx_buffer
		}
		else                                      //if received data = 13
		{
			data_length = rx_index;
			rx_index = 0;
			transfer_complete = 1;                 //transfer complete, data is ready to read
			packet_start = false;
		}
*/		
		
    HAL_UART_Receive_IT(&huart1, (uint8_t *)rx_data, 1);   //activate UART receive interrupt every time
	}
}




