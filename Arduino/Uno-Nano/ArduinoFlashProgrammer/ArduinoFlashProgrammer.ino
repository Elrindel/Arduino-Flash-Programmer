/**
 * A0     = A16
 * A1     = A17
 * A2     = A18
 * A3     = SER
 * A4     = CLK
 * A5     = RCLK
 *
 * 2      = D0
 * 3      = D1
 * 4      = D2
 * 5      = D3
 * 6      = D4
 * 7      = D5
 * 8      = D6
 * 9      = D7
 * 10     = CE
 * 11     = OE
 * 12     = WE
**/

uint32_t SERIAL_TIMEOUT = 200;

struct Command
{
	uint32_t address;
	byte data;
};
struct Sequence
{
	Command reset;

	uint8_t write_size;
	Command write[6];

	uint8_t erase_size;
	Command erase[6];

	uint8_t info_size;
	Command info[6];
};

uint8_t current_sequence_num = 0;
struct Sequence sequence;

boolean set_sequence(uint8_t num)
{
	switch(num){
		case 1: //5555/AA, 2AAA/55, 5555/A0
			sequence = {
				{0x0000, 0xf0},
				3, {{0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0xA0}},
				6, {{0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x80}, {0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x10}},
				3, {{0x5555, 0xAA}, {0x2AAA, 0x55}, {0x5555, 0x90}},
			};
		break;
		case 2: //555/AA, 2AA/55, 555/A0
			sequence = {
				{0x000, 0xf0},
				3, {{0x555, 0xAA}, {0x2AA, 0x55}, {0x555, 0xA0}},
				6, {{0x555, 0xAA}, {0x2AA, 0x55}, {0x555, 0x80}, {0x555, 0xAA}, {0x2AA, 0x55}, {0x555, 0x10}},
				3, {{0x555, 0xAA}, {0x2AA, 0x55}, {0x555, 0x90}},
			};
		break;
		default:
			return false;
	}
	current_sequence_num = num;
	return true;
}

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(SERIAL_TIMEOUT);

	set_sequence(1);

	DDRC = 0b00111111; //A0-A5 : OUTPUT

	DDRD = 0b11111100; //D2-D7 : OUTPUT
	DDRB = 0b00011111; //D8-D12 : OUTPUT

	PORTB = 0b00011100; //D10-D12 = HIGH

	Serial.println("READY");
}

void set_address(uint32_t address)
{
	PORTC &= 0b11011111; //A5 = LOW

	for(int8_t i = 15; i >= 0; i--){
		PORTC &= 0b11101111; //A4 = LOW
		PORTC = (PORTC&0b11110111)|(((address>>i)&1)<<3); //A3 = (address>>i)&1
		PORTC |= 0b00010000; //A4 = HIGH
	}

	PORTC = (PORTC&0b11111000)|((address>>16)&7)|0b00100000; //A0-A2 = (address>>16)&7; A5 = HIGH
}

void set_data_direction(int8_t out)
{
	if(out > 0){
		DDRD |= 0b11111100; //D2-D7 : OUTPUT
		DDRB |= 0b00000011; //D8-D9 : OUTPUT
	}else{
		DDRD &= 0b00000011; //D2-D7 : INPUT
		DDRB &= 0b11111100; //D8-D9 : INPUT
	}
}

void set_data(uint8_t data)
{
	PORTD = (PORTD&0b00000011)|(data<<2); //D2-D7 = data<<2
	PORTB = (PORTB&0b11111100)|(data>>6); //D8-D9 = data>>6
}

byte get_data()
{
	return (PIND>>2)|(PINB<<6);
}

void write_data(Command cmd)
{
	set_address(cmd.address);
	set_data_direction(1);
	PORTB |= 0b00011100; //D10-D12 = HIGH
	delayMicroseconds(1);
	PORTB &= 0b11101011; //D10,D12 = LOW
	set_data(cmd.data);
	PORTB |= 0b00010100; //D10,D12 = HIGH
	delayMicroseconds(1);
	set_data_direction(0);
}

byte read_data(uint32_t address)
{
	set_address(address);
	set_data_direction(0);

	PORTB = (PORTB&0b11110011)|0b00010000; //D10,D11 = LOW, D12 = HIGH
	delayMicroseconds(2);
	byte data = get_data();

	PORTB |= 0b00001100; //D10,D11 = HIGH
	return data;
}

int serial_timed_read()
{
	int c;
	unsigned long _startMillis = millis();
	do {
		c = Serial.read();
		if(c >= 0) return c;
	} while(millis() - _startMillis < SERIAL_TIMEOUT);
	return -1;
}

String serial_read_arg()
{
	String arg = "";
	int c;
	do{
		c = serial_timed_read();
		if(c == 0x0D)
			continue;
		if(c < 0 || c == 0x20 || c == 0x0A)
			break;
		arg += (char)c;
	}while(c >= 0);
	return arg;
}

int32_t arg_to_int(String arg, int32_t def = 0)
{
	if(arg.length() == 0){
		return def;
	}else if(arg[0] == 'x' || arg[0] == 'X'){
		return strtol(arg.c_str()+1, 0, 16);
	}else{
		return arg.toInt();
	}
}

void process_read(boolean &hex_output)
{
	int32_t address = arg_to_int(serial_read_arg(), -1);
	int32_t len = arg_to_int(serial_read_arg(), 1);
	if(address < 0 || len < 0){
		Serial.println("ERR");
		return;
	}
	for(uint32_t i = 0; i < (uint32_t)len; i++){
		if(hex_output){
			Serial.print(read_data(address), HEX);
			if(i+1 < (uint32_t)len)
				Serial.print(' ');
		}else{
			Serial.print((char)read_data(address));
		}
		address++;
	}
}

void process_write()
{
	int32_t address = arg_to_int(serial_read_arg(), -1);
	int32_t len = arg_to_int(serial_read_arg(), -1);
	if(address < 0 || len < 0){
		Serial.println("ERR");
		return;
	}
	Serial.println(">");
	uint32_t countWrite = 0;
	for(uint32_t i = 0; i < (uint32_t)len; i++){
		while(!Serial.available());
		for(int8_t wi = 0; wi < sequence.write_size; wi++){
			write_data(sequence.write[wi]);
		}
		write_data({(uint32_t)address, (byte)Serial.read()});
		delayMicroseconds(1);
		address++;
		countWrite++;
		if(countWrite%32 == 0)
			Serial.println(">");
	}
	if(countWrite == 0 || countWrite%32 != 0)
		Serial.println(">");
	Serial.print("OK ");
	Serial.println(countWrite);
}

void process_erase()
{
	for(int8_t i = 0; i < sequence.erase_size; i++){
		write_data(sequence.erase[i]);
	}
	Serial.println("OK");
}

void process_info(boolean &hex_output)
{
	write_data(sequence.reset);
	for(int8_t i = 0; i < sequence.info_size; i++){
		write_data(sequence.info[i]);
	}
	delayMicroseconds(1);

	if(hex_output){
		Serial.print(read_data(0x0000), HEX);
		Serial.print(' ');
		Serial.println(read_data(0x0001), HEX);
	}else{
		Serial.print((char)read_data(0x0000));
		Serial.println((char)read_data(0x0001));
	}

	write_data(sequence.reset);
}

void loop()
{ 
	while(!Serial.available());
	char cmd = Serial.read();
	String arg = "";
	boolean hex_output = false;

	if(cmd == 'X' || cmd == 'x'){
		hex_output = true;
		cmd = serial_timed_read();
	}

	switch(cmd)
	{
		case 'T':
			SERIAL_TIMEOUT += 2000;
			if(SERIAL_TIMEOUT > 14200)
				SERIAL_TIMEOUT = 200;
			Serial.setTimeout(SERIAL_TIMEOUT);
			Serial.print("OK ");
			Serial.println(SERIAL_TIMEOUT);
		break;
		case 'S':
			arg = serial_read_arg();
			if(arg.length() == 0){
				Serial.println(current_sequence_num);
			}else if(set_sequence(arg.toInt())){
				Serial.println("OK");
			}else{
				Serial.println("ERR");
			}
		break;
		case 'R':
			process_read(hex_output);
		break;
		case 'W':
			process_write();
		break;
		case 'E':
			process_erase();
		break;
		case 'I':
			process_info(hex_output);
		break;
	}
}
