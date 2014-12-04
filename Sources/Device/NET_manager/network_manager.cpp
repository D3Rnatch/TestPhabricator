
#include "network_manager.h"


		Network_manager::Network_manager(int baud)
		{
			Serial.begin(baud);
			Serial.flush();
			Serial.setTimeout(TIMEOUT); // See .h for details
			this->tmp_read_status = 0;
			this->tmp_write_status = 0;
			#ifdef DBG
				this->send_dbg_message_to_computer("Ok. Network initialised.");
			#endif
		}
	
		void Network_manager :: run_the_magic()
		{
			// Reading data :
			this->tmp_read_status = 0;
			Serial.readBytesUntil('\n',this->tmp_read, 64);
			for(int i=0;i<64;i++){
				if(this->tmp_read[i] != '\0')
					this->tmp_read_status += 1;
				else
					break;
			}
			
			// write Data :
			if(this->tmp_write_status != 0)
			{
				for(int i=0; i<this->tmp_write_status; i++)
				{
					this->send_data_to_computer(this->tmp_write[i])
					this->tmp_write[i] = '\0';
				}
				this->tmp_write_status = 0;
			}
		}
		
		char Network_manager :: read_data_from_buffer()
		{
			// TO BE IMPLEMENTED
		}
		
		void Network_manager :: write_data_to_buffer(char *b, int s)
		{
			// TO BE IMPLEMENTED
		}
	
		void Network_manager :: send_dbg_message_to_computer(char * t)
		{
			Serial.println(t);
		}
		
		void Network_manager :: send_dbg_message_to_user()
		{
			// TO BE IMPLEMENTED
		}
		
		void Network_manager :: send_data_to_computer(char v)
		{
			// TO BE IMPLEMENTED
		}
		
		void Network_manager :: send_data_to_user(char i)
		{
			// TO BE IMPLEMENTED
		}
