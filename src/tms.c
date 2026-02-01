/*
  vim:ts=4
  vim:sw=4
*/
#include "tms.h"

static uint8_t VDPMode = 0;

extern uint8_t CHAR_TABLE[128][8];
extern uint8_t VDPModeTable[32];

static uint8_t ScreenMode0[520];

bool tms_init()
{
    // No reset pin, unfortunately!
    // CONTROL : Pins 4,5,6,7 are always write
    io_out(TMS_SIG_DIR, TMS_SIG_INIT);
    //IO(TMS_SIG_DIR) = TMS_SIG_INIT;

    // DATA : can be Read or Write, depending. Start with READ
    io_out(TMS_DATA_DIR, TMS_DATA_READ);
    //IO(TMS_DATA_DIR) = TMS_DATA_READ;

    // set registers for mode 0
    VDPMode = 0; // default
    for (uint8_t i=0; i<8; i++)
    {
        tms_write_vdp_register( i, VDPModeTable[i] );
    }

    return true;
}


// Write 8 bits to TMS VDP data port
void tms_write_data(uint8_t data)
{
    // Set Control signals
    uint8_t signals = 0;
    
    // RWB & A1 - low
    // MODE - low : Selects Data word
    // EN - low

    signals = TMS_SIG_RWB_RESET | TMS_SIG_A1_RESET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_RESET; 
    
    io_out(TMS_SIG, signals);

    // Set Data to WRITE
    io_out(TMS_DATA_DIR, TMS_DATA_WRITE);
    //IO(TMS_DATA_DIR) = TMS_DATA_WRITE;
    io_out(TMS_DATA, data);
    //IO(TMS_DATA) = data;

    // Set the Enable and RWB back to 1
    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_SET; 

    io_out(TMS_SIG, signals);
}

// Read 8 bits from TMS VDP data port
uint8_t tms_read_data()
{
    uint8_t data = 0;
    uint8_t signals = 0;
    
    // Set Control signals
    
    // RWB & A1 - High
    // MODE - low : Selects Data word
    // EN - low

    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_RESET; 

    io_out(TMS_SIG, signals);

    // Set Data to READ
    io_out(TMS_DATA_DIR, TMS_DATA_READ);
    //IO(TMS_DATA_DIR) = TMS_DATA_READ;
    
    // delay here?
    
    data = io_in(TMS_DATA);
    //data = IO(TMS_DATA);

    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_SET; 

    io_out(TMS_SIG, signals);

    return data;
}

// Write a byte to the Control/Address word
void tms_write_ctrl_addr(uint8_t data)
{
    // Set Control signals
    uint8_t signals = 0;
    
    // RWB & A1 - low
    // MODE - high : Selects Control/Address word
    // EN - low

    signals = TMS_SIG_RWB_RESET | TMS_SIG_A1_RESET |
              TMS_SIG_MODE_SET |
              TMS_SIG_EN_RESET; 

    io_out(TMS_SIG, signals);

    // Set Data to WRITE
    io_out(TMS_DATA_DIR, TMS_DATA_WRITE);
    //IO(TMS_DATA_DIR) = TMS_DATA_WRITE;
    io_out(TMS_DATA, data);
    //IO(TMS_DATA) = data;

    // Set the Enable and RWB back to 1
    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_SET; 

    io_out(TMS_SIG, signals);
}

// Read from the VDP status register
uint8_t tms_read_status()
{
    uint8_t data = 0;
    uint8_t signals = 0;
    
    // Set Control signals
    
    // RWB & A1 - High
    // MODE - high : Selects Ctrol/Status word
    // EN - low

    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_SET |
              TMS_SIG_EN_RESET; 

    io_out(TMS_SIG, signals);

    // Set Data to READ
    io_out(TMS_DATA_DIR, TMS_DATA_READ);
    //IO(TMS_DATA_DIR) = TMS_DATA_READ;
    
    // delay here?
    
    data = io_in(TMS_DATA);
    //data = IO(TMS_DATA);

    // Set the Enable and RWB back to 1
    signals = TMS_SIG_RWB_SET | TMS_SIG_A1_SET |
              TMS_SIG_MODE_RESET |
              TMS_SIG_EN_SET; 

    io_out(TMS_SIG, signals);

    return data;
}

//  Table 2-1 from TMS9918A Datasheet
// 
// +--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------+
// |                    |                    Bits                       |     | !a1 |  a0  |
// |    Operation       |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  | CSW | CSR | MODE |
// |--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------|
// |Write to VRAM       |     |     |     |     |     |     |     |     |     |     |      |
// |  Byte 1 Addr Setup | A6  | A7  | A8  | A9  | A10 | A11 | A12 | A13 |  0  |  1  |  1   |
// |  Byte 2 Addr Setup |  0  |  1  | A0  | A1  | A2  | A3  | A4  | A5  |  0  |  1  |  1   |
// |  Byte 3 Data Write | D0  | D1  | D2  | D3  | D4  | D5  | D6  | D7  |  0  |  1  |  0   |
// |--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------|
// |VDP Register Write  |     |     |     |     |     |     |     |     |     |     |      |
// |  Byte 1 Data Write | D0  | D1  | D2  | D3  | D4  | D5  | D6  | D7  |  0  |  1  |  1   |
// |  Byte 2 Reg Write  |  1  |  0  |  0  |  0  |  0  | rs0 | rs1 | rs2 |  0  |  1  |  1   |
// |--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------|
// |Read from VRAM      |     |     |     |     |     |     |     |     |     |     |      |
// |  Byte 1 Addr Setup | A6  | A7  | A8  | A9  | A10 | A11 | A12 | A13 |  0  |  1  |  1   |
// |  Byte 2 Addr Setup |  0  |  0  | A0  | A1  | A2  | A3  | A4  | A5  |  0  |  1  |  1   |
// |  Byte 3 Data Read  | D0  | D1  | D2  | D3  | D4  | D5  | D6  | D7  |  1  |  0  |  0   |
// |--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------|
// |Read VDP Status     |     |     |     |     |     |     |     |     |     |     |      |
// |  Byte 1 Data Read  | D0  | D1  | D2  | D3  | D4  | D5  | D6  | D7  |  1  |  0  |  1   |
// +--------------------+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+------+

// Write data to one of seven VDP registers
void tms_write_vdp_register(uint8_t reg, uint8_t data)
{
    vdp_set_text_colour(7); printf("Write VDP Reg %d -> %02X\n", reg, data);
    tms_write_data( data );
    tms_write_ctrl_addr( reg | 0x80 );
}

// Read VDP status register
uint8_t tms_read_vdp_status()
{
    uint8_t status = tms_read_status();
    vdp_set_text_colour(7); printf("Read VDP Status %02X\n", status);
    return status;
}

// Set VDP address for reading or writing.
// VDP Addresses are 14-bit (hi byte first)
//  - Follow by a series of writes/reads to/from VRAM
void tms_set_vdp_address(uint16_t vaddr)
{
    vdp_set_text_colour(7); printf("Set VDP Addr %04X\n", vaddr);
    uint8_t lobyte=0, hibyte=0;
    lobyte = (vaddr & 0xFF);
    hibyte = ((vaddr>>8) & 0x3F) | 0x40;
    tms_write_data( lobyte );
    tms_write_data( hibyte );
}

// Write a data block to the VDP
void tms_vdp_write_data(uint16_t vaddr, uint16_t datalen, uint8_t *data)
{
    tms_set_vdp_address(vaddr);
    for(uint16_t i=0; i<datalen; i++)
    {
        tms_write_data(data[i]);
    }
}

// Read a block of data from the VDP
void tms_vdp_read_data(uint16_t vaddr, uint16_t datalen, uint8_t* buffer)
{
    tms_set_vdp_address(vaddr);
    for(uint16_t i=0; i<datalen; i++)
    {
        buffer[i] = tms_read_data();
    }
}

// Set VDP mode 
// 0 = text mode, no sprites
// 1 = Graphics mode I
// 2 = Graphics mode II
// 3 = Multicolor mode
void tms_vdp_set_mode(uint8_t mode)
{
    vdp_set_text_colour(7); printf("Set mode %d\n", mode);
    uint16_t tbl_offset = mode << 3; // Offset into mode register table

    for (uint8_t i=0; i<8; i++)
    {
        tms_write_vdp_register( i, VDPModeTable[ (VDPMode<<3) + i] );
    }
}

void tms_vdp_load_character_set()
{
    // Load default character set
    for (uint8_t c=0; c<128; c++)
    {
        tms_vdp_write_data( (VDPMode<<3) + VDP_ADDR_PATT_TABLE, 8, CHAR_TABLE[c]);
    }
}

void tms_vdp_clear_screen_m0( uint8_t clr_char )
{
    // Mode 0 is 40x24 - 520 characters
   
    for (int i=0; i<520; i++)
    {
        ScreenMode0[i] = clr_char;
    }
    tms_vdp_write_data( VDP_ADDR_NAME_TABLE, 520, ScreenMode0 );
}

void tms_memory_test(void)
{
    int num_bytes = 16;
    // Write to VDP address 0x00 and read back

    vdp_set_text_colour(7); printf("Write %d bytes\n", num_bytes);
    tms_set_vdp_address(0x00);
    for (unsigned int i=0; i<num_bytes; i++)
    {
        tms_write_data(i);
    }
    vdp_set_text_colour(7); printf("Read back\n");
    tms_set_vdp_address(0x00);
    for (unsigned int i=0; i < num_bytes; i++)
    {
        vdp_set_text_colour(6); printf("0x%02X ",tms_read_data());
    }

}
