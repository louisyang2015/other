//here are the constants available throughout the application

#ifndef LOUIS_GLOBAL_CONSTANTS_H
#define LOUIS_GLOBAL_CONSTANTS_H

//------------------------------------------------------------
//IDs for the components used in this program

//see the test program's output in component.html
//documentation to see what pictures these IDs are
//referring to

//first, I will give the class_id
//then the number of types for this class_id
//then each of the type_id constants

//example: P_TRANSISTOR has class ID of 3
//there are 2 types of P_TRANSISTOR --- the BJT (bipolar
//junction transistor) and FET(field effect transistor)
//BJT type_id is 1
//FET type_id is 2
const int CLASS_ID_P_TRANSISTOR = 3;
const int TYPES_OF_P_TRANSISTOR = 4;
const int TYPE_ID_PNP_BJT = 1;
const int TYPE_ID_PNP_BJT2 = 2;
const int TYPE_ID_PMOS = 3;
const int TYPE_ID_PMOS2 = 4;
//end of example

const int CLASS_ID_RESISTOR = 1;
const int TYPES_OF_RESISTOR = 1;
const int TYPE_ID_RESISTOR = 1;

const int CLASS_ID_DIODE = 2;
const int TYPES_OF_DIODE = 2;
const int TYPE_ID_NORMAL_DIODE = 1;
const int TYPE_ID_ZENER_DIODE = 2;

const int CLASS_ID_N_TRANSISTOR = 4;
const int TYPES_OF_N_TRANSISTOR = 4;
const int TYPE_ID_NPN_BJT = 1;
const int TYPE_ID_NPN_BJT2 = 2;
const int TYPE_ID_NMOS = 3;
const int TYPE_ID_NMOS2 = 4;

const int CLASS_ID_BLACKBOX = 5;
const int TYPES_OF_BLACKBOX = 2;
const int TYPE_ID_BASIC_BOX = 1;
const int TYPE_ID_2PORT_BOX = 2;

const int CLASS_ID_INDUCTOR = 6;
const int TYPES_OF_INDUCTOR = 1;
const int TYPE_ID_INDUCTOR = 1;

const int CLASS_ID_CAPACITOR = 7;
const int TYPES_OF_CAPACITOR = 1;
const int TYPE_ID_CAPACITOR = 1;

const int CLASS_ID_VOLTAGE_SOURCE = 8;
const int TYPES_OF_VOLTAGE_SOURCE = 6;
const int TYPE_ID_VS_DC = 1;
const int TYPE_ID_VS_GENERAL = 2;
const int TYPE_ID_VS_DC_BIG = 3;
const int TYPE_ID_VS_AC = 4;
const int TYPE_ID_VS_DEP = 5;
const int TYPE_ID_VS_ARROW = 6;


const int CLASS_ID_CURRENT_SOURCE = 9;
const int TYPES_OF_CURRENT_SOURCE = 2;
const int TYPE_ID_CS_DC = 1;
const int TYPE_ID_CS_DEP = 2;

const int CLASS_ID_GROUND = 10;
const int TYPES_OF_GROUND = 1;
const int TYPE_ID_GROUND = 1;

const int CLASS_ID_WIRE = 11;
const int TYPES_OF_WIRE = 9;
const int TYPE_ID_WIRE_BASIC = 1;
const int TYPE_ID_WIRE_ANGLE = 2;
const int TYPE_ID_WIRE_TRI = 3;
const int TYPE_ID_WIRE4_1 = 4;	//one wire loops over another
const int TYPE_ID_WIRE4_2 = 5;	//just a simple cross
const int TYPE_ID_WIRE4_3 = 6;	//4 wires meeting a node
//these next constants are added on later --- to deal with
//diagonal wires
const int TYPE_ID_WIRE_SINGLE_END = 7;
const int TYPE_ID_WIRE_ANGLE2 = 8;
const int TYPE_ID_WIRE_ANGLE3 = 9;

const int CLASS_ID_OPAMP = 12;
const int TYPES_OF_OPAMP = 1;
const int TYPE_ID_OPAMP = 1;

//End of IDs for the components used in this program
//------------------------------------------------------------


//------------------------------------------------------------
//constants about the Grid.h 

// first, the Grid class's hash table
const int HASH_TABLE_SIZE = 16 * 1024; //16K
const int ADDRESS_MASK = 127; // binary: 0111 1111
const int ADDRESS_MASK_SIZE = 7; //7 bit address

//how many tiles per screen
const int MIN_X_TILES_PER_SCREEN = 1;
const int MAX_X_TILES_PER_SCREEN = 40;
const int MIN_Y_TILES_PER_SCREEN = 1;
const int MAX_Y_TILES_PER_SCREEN = 30;

//the range of "Tile" coordinates --- used for both x and y
//note that we are not using the absolute minimum and the absolute
//maximum --- this lets us detect over flow and under flow more
//easily
const int MIN_TILE_COORD = 0 + MAX_X_TILES_PER_SCREEN * 512;
const int MAX_TILE_COORD = 0x7FFFFFFF - MAX_X_TILES_PER_SCREEN * 512;


//end of constants about the Grid class
//------------------------------------------------------------


//------------------------------------------------------------
//constants for the ImageList Class (ImageList.h)
const int IMAGE_LIST_MAX_SIZE = 100;
//------------------------------------------------------------

//------------------------------------------------------------
//constants for the UserSelection class (UserSelection.h)
const int CLASS_ID_LIST_MAX_SIZE = 25;
//------------------------------------------------------------

//------------------------------------------------------------
//constants for the Pane class (Pane.h)
const int PANE_TILES_HORZ = 2;
const int PANE_TILES_VERT = 5;
const int PANE_MAX_SELECTION = PANE_TILES_HORZ * PANE_TILES_VERT - 1;
//------------------------------------------------------------

#endif
