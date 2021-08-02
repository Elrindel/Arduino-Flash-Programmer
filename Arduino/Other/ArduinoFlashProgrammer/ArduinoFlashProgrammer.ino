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

	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(A4, OUTPUT);
	pinMode(A5, OUTPUT);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);

	digitalWrite(10, HIGH);
	digitalWrite(11, HIGH);
	digitalWrite(12, HIGH);

	Serial.println("READY");
}

void set_address(uint32_t address)
{
	digitalWrite(A5, LOW);

	for(int8_t i = 15; i >= 0; i--){
		digitalWrite(A4, LOW);
		digitalWrite(A3, (address>>i)&1);
		digitalWrite(A4, HIGH);
	}

	digitalWrite(A0, (address>>16)&1);
	digitalWrite(A1, (address>>17)&1);
	digitalWrite(A2, (address>>18)&1);
	digitalWrite(A5, HIGH);
}

void set_data_direction(int8_t out)
{
	if(out > 0){
		pinMode(2, OUTPUT);
		pinMode(3, OUTPUT);
		pinMode(4, OUTPUT);
		pinMode(5, OUTPUT);
		pinMode(6, OUTPUT);
		pinMode(7, OUTPUT);
		pinMode(8, OUTPUT);
		pinMode(9, OUTPUT);
	}else{
		pinMode(2, INPUT);
		pinMode(3, INPUT);
		pinMode(4, INPUT);
		pinMode(5, INPUT);
		pinMode(6, INPUT);
		pinMode(7, INPUT);
		pinMode(8, INPUT);
		pinMode(9, INPUT);
	}
}

void set_data(uint8_t data)
{
	for(uint8_t i = 0; i < 8; i++){
		digitalWrite(i+2, (data>>i)&1);
	}
}

byte get_data()
{
	byte data = 0;
	for(uint8_t i = 0; i < 8; i++){
		data |= digitalRead(i+2)<<i;
	}
	return data;
}

void write_data(Command cmd)
{
	set_address(cmd.address);
	set_data_direction(1);
	digitalWrite(10, HIGH);
	digitalWrite(11, HIGH);
	digitalWrite(12, HIGH);
	delayMicroseconds(1);
	digitalWrite(10, LOW);
	digitalWrite(12, LOW);
	set_data(cmd.data);
	digitalWrite(12, HIGH);
	digitalWrite(10, HIGH);
	delayMicroseconds(1);
	set_data_direction(0);
}

byte read_data(uint32_t address)
{
	set_address(address);
	set_data_direction(0);

	digitalWrite(11, LOW);
	digitalWrite(12, HIGH);
	digitalWrite(10, LOW);
	delayMicroseconds(2);
	byte data = get_data();
	digitalWrite(11, HIGH);
	digitalWrite(10, HIGH);
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
