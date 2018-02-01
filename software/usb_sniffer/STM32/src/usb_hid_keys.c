//usb_hid_keys.c

#include "hal.h"
#include "usb_hid_keys.h"

#define NOP '?'

static char azerty[120] = {
    NOP,   //0x00
    NOP,   //0x01
    NOP,   //0x02
    NOP,   //0x03
    'q',   //0x04 q | Q | @
    'b',   //0x05 b | B | ”
    'c',   //0x06 c | C | ¢
    'd',   //0x07 d | D | ð
    'e',   //0x08 e | E | €
    'f',   //0x09 f | F | đ
    'g',   //0x0a g | G | ŋ
    'h',   //0x0b h | H | ħ
    'i',   //0x0c i | I | →
    'j',   //0x0d j | J |
    'k',   //0x0e k | K | ĸ
    'l',   //0x0f l | L | ł
    ',',   //0x10 , | , | ´
    'n',   //0x11 n | N | n
    'o',   //0x12 o | O | ø
    'p',   //0x13 p | P | þ
    'a',   //0x14 a | A | æ
    'r',   //0x15 r | R | ¶
    's',   //0x16 s | S | ß
    't',   //0x17 t | T | ŧ
    'u',   //0x18 u | U | ↓
    'v',   //0x19 v | V | “
    'z',   //0x1a z | Z | «
    'x',   //0x1b x | X | »
    'y',   //0x1c y | Y | ←
    'w',   //0x1d w | W | ł
    '&',   //0x1e & | 1 | ¹
    'e',   //0x1f é | 2 | ~
    '"',   //0x20 " | 3 | #
    '\'',  //0x21 ' | 4 | {
    '(',   //0x22 ( | 5 | [
    '-',   //0x23 - | 6 | |
    'e',   //0x24 è | 7 | `
    '_',   //0x25 _ | 8 | back slash
    'c',   //0x26 ç | 9 | ^
    'a',   //0x27 à | 0 | @
    '\n',  //0x28 Return
    '=',   //0x29
    '\b',  //0x2a Back space
    '\t',  //0x2b Tab
    ' ',   //0x2c Space
    ')',   //0x2d ) | ° | ]
    '=',   //0x2e = | + | }
    '^',   //0x2f ^ | ¨ | ¨
    '$',   //0x30 $ | £ | ¤
    '*',   //0x31 * | µ | `
    '*',   //0x32 * | µ | `
    'm',   //0x33 m | M | µ
    'u',   //0x34 ù | % | ^
    '2',   //0x35 ² | ~ | ¬
    ';',   //0x36 ; | . |
    ':',   //0x37 : | / | ·
    '!',   //0x38 ! | § | ̣̣
    NOP,   //0x39
    NOP,   //0x3a F1
    NOP,   //0x3b F2
    NOP,   //0x3c F3
    NOP,   //0x3d F4
    NOP,   //0x3e F5
    NOP,   //0x3f F6
    NOP,   //0x40 F7
    NOP,   //0x41 F8
    NOP,   //0x42 F9
    NOP,   //0x43 F10
    NOP,   //0x44 F11
    NOP,   //0x45 F12
    NOP,   //0x46 Screen shot
    NOP,   //0x47 Scroll lock
    NOP,   //0x48 Stop
    NOP,   //0x49 insert
    NOP,   //0x4a Home
    NOP,   //0x4b page up
    NOP,   //0x4c supp
    NOP,   //0x4d end
    NOP,   //0x4e page down
    NOP,   //0x4f right
    NOP,   //0x50 left
    NOP,   //0x51 down
    NOP,   //0x52 up
    NOP,   //0x53 NUM lock
    '/',   //0x54
    '*',   //0x55
    '-',   //0x56
    '+',   //0x57
    '\n',  //0x58 Enter
    '1',   //0x59 1 | end
    '2',   //0x5a 2 | down
    '3',   //0x5b 3 | page down
    '4',   //0x5c 4 | left
    '5',   //0x5d 5 |
    '6',   //0x5e 6 | right
    '7',   //0x5f 7 | home
    '8',   //0x60 8 | up
    '9',   //0x61 9 | page up
    '0',   //0x62 0 | insert
    '.',   //0x63 . | supp
    '<',   //0x64 < | >
    NOP    //0x65 Right clic button
};

static char azerty_maj[120] = {
    NOP,   //0x00
    NOP,   //0x01
    NOP,   //0x02
    NOP,   //0x03
    'Q',   //0x04 q | Q | @
    'B',   //0x05 b | B | ”
    'C',   //0x06 c | C | ¢
    'D',   //0x07 d | D | ð
    'E',   //0x08 e | E | €
    'F',   //0x09 f | F | đ
    'G',   //0x0a g | G | ŋ
    'H',   //0x0b h | H | ħ
    'I',   //0x0c i | I | →
    'J',   //0x0d j | J |
    'K',   //0x0e k | K | ĸ
    'L',   //0x0f l | L | ł
    '?',   //0x10 , | ? | ´
    'N',   //0x11 n | N | n
    'O',   //0x12 o | O | ø
    'P',   //0x13 p | P | þ
    'A',   //0x14 a | A | æ
    'R',   //0x15 r | R | ¶
    'S',   //0x16 s | S | ß
    'T',   //0x17 t | T | ŧ
    'U',   //0x18 u | U | ↓
    'V',   //0x19 v | V | “
    'Z',   //0x1a z | Z | «
    'X',   //0x1b x | X | »
    'Y',   //0x1c y | Y | ←
    'W',   //0x1d w | W | ł
    '1',   //0x1e & | 1 | ¹
    '2',   //0x1f é | 2 | ~
    '3',   //0x20 " | 3 | #
    '4',   //0x21 ' | 4 | {
    '5',   //0x22 ( | 5 | [
    '6',   //0x23 - | 6 | |
    '7',   //0x24 è | 7 | `
    '8',   //0x25 _ | 8 | back slash
    '9',   //0x26 ç | 9 | ^
    '0',   //0x27 à | 0 | @
    '\n',  //0x28 Return
    NOP,   //0x29
    '\b',  //0x2a Back space
    '\t',  //0x2b Tab
    ' ',   //0x2c Space
    NOP,   //0x2d ) | ° | ]
    '+',   //0x2e = | + | }
    NOP,   //0x2f ^ | ¨ | ¨
    NOP,   //0x30 $ | £ | ¤
    NOP,   //0x31 * | µ | `
    NOP,   //0x32 * | µ | `
    'M',   //0x33 m | M | µ
    '%',   //0x34 ù | % | ^
    '~',   //0x35 ² | ~ | ¬
    '.',   //0x36 ; | . |
    '/',   //0x37 : | / | ·
    NOP,   //0x38 ! | § | ̣̣
    NOP,   //0x39
    NOP,   //0x3a F1
    NOP,   //0x3b F2
    NOP,   //0x3c F3
    NOP,   //0x3d F4
    NOP,   //0x3e F5
    NOP,   //0x3f F6
    NOP,   //0x40 F7
    NOP,   //0x41 F8
    NOP,   //0x42 F9
    NOP,   //0x43 F10
    NOP,   //0x44 F11
    NOP,   //0x45 F12
    NOP,   //0x46 Screen shot
    NOP,   //0x47 Scroll lock
    NOP,   //0x48 Stop
    NOP,   //0x49 insert
    NOP,   //0x4a Home
    NOP,   //0x4b page up
    NOP,   //0x4c supp
    NOP,   //0x4d end
    NOP,   //0x4e page down
    NOP,   //0x4f right
    NOP,   //0x50 left
    NOP,   //0x51 down
    NOP,   //0x52 up
    NOP,   //0x53 NUM lock
    '/',   //0x54
    '*',   //0x55
    '-',   //0x56
    '+',   //0x57
    '\n',  //0x58 Enter
    NOP,   //0x59 1 | end
    NOP,   //0x5a 2 | down
    NOP,   //0x5b 3 | page down
    NOP,   //0x5c 4 | left
    NOP,   //0x5d 5 |
    NOP,   //0x5e 6 | right
    NOP,   //0x5f 7 | home
    NOP,   //0x60 8 | up
    NOP,   //0x61 9 | page up
    NOP,   //0x62 0 | insert
    NOP,   //0x63 . | supp
    '>',   //0x64 < | >
    NOP    //0x65 Right clic button
};

static char azerty_alt[120] = {
    NOP,   //0x00
    NOP,   //0x01
    NOP,   //0x02
    NOP,   //0x03
    '@',   //0x04 q | Q | @
    '\"',  //0x05 b | B | ”
    NOP,   //0x06 c | C | ¢
    NOP,   //0x07 d | D | ð
    NOP,   //0x08 e | E | €
    NOP,   //0x09 f | F | đ
    NOP,   //0x0a g | G | ŋ
    NOP,   //0x0b h | H | ħ
    NOP,   //0x0c i | I | →
    NOP,   //0x0d j | J |
    NOP,   //0x0e k | K | ĸ
    NOP,   //0x0f l | L | ł
    NOP,   //0x10 , | , | ´
    NOP,   //0x11 n | N | n
    NOP,   //0x12 o | O | ø
    NOP,   //0x13 p | P | þ
    NOP,   //0x14 a | A | æ
    NOP,   //0x15 r | R | ¶
    NOP,   //0x16 s | S | ß
    NOP,   //0x17 t | T | ŧ
    NOP,   //0x18 u | U | ↓
    NOP,   //0x19 v | V | “
    NOP,   //0x1a z | Z | «
    NOP,   //0x1b x | X | »
    NOP,   //0x1c y | Y | ←
    NOP,   //0x1d w | W | ł
    NOP,   //0x1e & | 1 | ¹
    '~',   //0x1f é | 2 | ~
    '#',   //0x20 " | 3 | #
    '{',   //0x21 ' | 4 | {
    '[',   //0x22 ( | 5 | [
    '|',   //0x23 - | 6 | |
    '`',   //0x24 è | 7 | `
    '\\',  //0x25 _ | 8 | back slash
    '^',   //0x26 ç | 9 | ^
    '@',   //0x27 à | 0 | @
    '\n',  //0x28 Return
    '=',   //0x29
    '\b',  //0x2a Back space
    '\t',  //0x2b Tab
    ' ',   //0x2c Space
    ']',   //0x2d ) | ° | ]
    '}',   //0x2e = | + | }
    NOP,   //0x2f ^ | ¨ | ¨
    NOP,   //0x30 $ | £ | ¤
    '`',   //0x31 * | µ | `
    '`',   //0x32 * | µ | `
    NOP,   //0x33 m | M | µ
    '^',   //0x34 ù | % | ^
    NOP,   //0x35 ² | ~ | ¬
    NOP,   //0x36 ; | . |
    NOP,   //0x37 : | / | ·
    ' ',   //0x38 ! | § | ̣̣
    NOP,   //0x39
    NOP,   //0x3a F1
    NOP,   //0x3b F2
    NOP,   //0x3c F3
    NOP,   //0x3d F4
    NOP,   //0x3e F5
    NOP,   //0x3f F6
    NOP,   //0x40 F7
    NOP,   //0x41 F8
    NOP,   //0x42 F9
    NOP,   //0x43 F10
    NOP,   //0x44 F11
    NOP,   //0x45 F12
    NOP,   //0x46 Screen shot
    NOP,   //0x47 Scroll lock
    NOP,   //0x48 Stop
    NOP,   //0x49 insert
    NOP,   //0x4a Home
    NOP,   //0x4b page up
    NOP,   //0x4c supp
    NOP,   //0x4d end
    NOP,   //0x4e page down
    NOP,   //0x4f right
    NOP,   //0x50 left
    NOP,   //0x51 down
    NOP,   //0x52 up
    NOP,   //0x53 NUM lock
    '/',   //0x54
    '*',   //0x55
    '-',   //0x56
    '+',   //0x57
    '\n',  //0x58 Enter
    '1',   //0x59 1 | end
    '2',   //0x5a 2 | down
    '3',   //0x5b 3 | page down
    '4',   //0x5c 4 | left
    '5',   //0x5d 5 |
    '6',   //0x5e 6 | right
    '7',   //0x5f 7 | home
    '8',   //0x60 8 | up
    '9',   //0x61 9 | page up
    '0',   //0x62 0 | insert
    '.',   //0x63 . | supp
    '<',   //0x64 < | >
    NOP    //0x65 Right clic button
};

bool           caps_lock, num_lock, scroll_lock;
static uint8_t last_input[6];

static bool find(uint8_t input)
{
    for(int i = 0; i < 6; i++)
        if(input == last_input[i])
            return true;
    return false;
}

void get_input_hid(uint8_t *report, uint16_t *input)
{
    int index_input = 0;

    for(int i = 2; i < 8; i++) {
        if(report[i] == 0 || report[i] == 1)
            break;
        else if(!find(report[i])) {
            input[index_input] = ((uint16_t)report[0]) << 8 | (uint16_t)report[i];
            index_input++;
        }
    }

    //copy report for next input
    for(int i         = 0; i < 6; i++)
        last_input[i] = report[i + 2];
}

char hid_to_azerty(uint16_t input)
{
    uint8_t modifier = (input >> 8);
    uint8_t key      = (uint8_t)input;

    //no CTRL, WIN and ALT
    if((modifier == KEY_MOD_LSHIFT || modifier == KEY_MOD_RSHIFT ||
        modifier == KEY_MOD_RALT || modifier == 0) &&
       key != 0) {
        //no NUM lock
        if(!num_lock && key >= 0x59 && key <= 0x63)
            return 0x00;
        //MAJ lock
        else if(((modifier == KEY_MOD_LSHIFT || modifier == KEY_MOD_RSHIFT) && !caps_lock) ||
                ((modifier != KEY_MOD_LSHIFT && modifier != KEY_MOD_RSHIFT) && caps_lock))
            return azerty_maj[key];
        //ALT Gr pressed
        else if(modifier == KEY_MOD_RALT)
            return azerty_alt[key];
        //no modifier
        else
            return azerty[key];
    }

    return 0x00;
}
