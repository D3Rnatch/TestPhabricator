#include "network_manager.h"

Network_manager::Network_manager(int baud)
{
    Serial.begin(baud);
    Serial.flush();
    Serial.setTimeout(TIMEOUT); // See .h for details
    this->last_extraction.type = -1;
    for(int i=0;i<10;i++)
    this->encapsulation_stack[i].array[0] = 255;
    this->flag = false;
}
	
void Network_manager :: run_the_magic()
{
    this->flag = false;
    // Reading data :
    // Serial.flush();
    char value[7];
    Serial.readBytes(value,7);
    Serial.flush();
    //Serial.print("ACQQ : ");
    //Serial.print(value);
    //Serial.print("\n");
    if(value[6] == '\n') // il s'agit d'une frame
    {
        //Serial.println("FRAME !");
        this->flag = true;
	this->last_extraction = extract_data_bytes((byte*)value);
    }
}
		
int Network_manager :: get_last_frame_id()
{
    if (this->flag)
	return this->last_extraction.type;	
    else 
        return 200;
}
		
uint8_t * Network_manager :: get_array()
{
    if (this->flag)
        return this->last_extraction.array;
    else 
        return NULL;	
}
	
void Network_manager :: send(byte b0,byte b1,byte b2,byte b3,byte b4,byte b5)
{
    this->encapsulation_stack[0].array[0] = b0;
    this->encapsulation_stack[0].array[1] = b1;
    this->encapsulation_stack[0].array[2] = b2;
    this->encapsulation_stack[0].array[3] = b3;
    this->encapsulation_stack[0].array[4] = b4;
    for(int i=0;i<5;i++) {
	Serial.write(this->encapsulation_stack[0].array[i]);
	//Serial.print(":"); 
     }
     Serial.write('\n');
}

void Network_manager :: send_full(uint8_t id,byte b0,byte b1,byte b2,byte b3,byte b4,byte b5)
{
    this->encapsulation_stack[0].array[0] = b0;
    this->encapsulation_stack[0].array[1] = b1;
    this->encapsulation_stack[0].array[2] = b2;
    this->encapsulation_stack[0].array[3] = b3;
    this->encapsulation_stack[0].array[4] = b4;
    Serial.write(id);
    for(int i=0;i<5;i++) {
	Serial.write(this->encapsulation_stack[0].array[i]);
	//Serial.print(":"); 
    }
    Serial.write('\n');
}

void Network_manager :: send_packet(t_encap * packet)
{
    for(int i=0;i<6;i++) {
        this->encapsulation_stack[0].array[0] = packet->array[i];
        Serial.write(packet->array[i]+48); }
    Serial.write('\n');
}

void Network_manager :: send_ready_packet(uint8_t errorCode)
{
    t_encap t = encap_ready();
    t.array[1] = (byte)errorCode;
    this->send_packet(&t);
}

void Network_manager :: send_data_packet(uint8_t v1, uint8_t v2, uint8_t v3)
{
	t_encap t = encap_acq_data(v1,v2,v3);
	this->send_packet(&t);
}
