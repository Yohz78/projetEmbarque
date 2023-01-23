#include "i2c.h"


    /**
     * @brief Construct a new i2c::i2c object
     * 
     * @param adresseI2C 
     * @param idBusI2C 
     */
    i2c::i2c(int adresseI2C, int idBusI2C){
        char filename[20];
        union i2c_smbus_data data ;

        snprintf(filename, 19, "/dev/i2c-%d", idBusI2C);
        if ((fd = open (filename, O_RDWR)) < 0){
            cout << "Erreur d'ouverture du bus I2C" << endl;
            exit(1);
        }
        //  Change slave address. The address is passed in the
        //  7 lower bits of the  argument
        if (ioctl (fd, I2C_SLAVE, adresseI2C) < 0){
            cout << "Impossible de sélectionner l'adresse I2C" << endl ;
            exit(1);
        }
        // Testing if the compenent is on the bus
        if (i2c_smbus_access (I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data)){
	    error = true ;
	}
	else{
	    error = false ;
	}
    }


    /**
     * @brief Return Error
     * 
     * @return true 
     * @return false 
     */
    bool i2c::getError(){
	return error;
    }

    /**
     * @brief Access to bus
     * 
     * @param rw 
     * @param command 
     * @param size 
     * @param data 
     * @return int 
     */
    int i2c::i2c_smbus_access (char rw, uint8_t command, int size, union i2c_smbus_data *data)
    {
        struct i2c_smbus_ioctl_data args ;

        args.read_write = rw ;
        args.command    = command ;
        args.size       = size ;
        args.data       = data ;
        return ioctl (fd, I2C_SMBUS, &args) ;
    }

    /**
     * @brief Read data from bus
     * 
     * @return unsigned char 
     */
    
    unsigned char i2c::Read(){
        union i2c_smbus_data data ;

        if (i2c_smbus_access (I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
        {
            error = true ;
            return 0x0;
        }
        else
            return data.byte & 0xFF ;

    }

    /**
     * @brief Read 8 bit register
     * 
     * @param reg 
     * @return unsigned char 
     */
    unsigned char i2c::ReadReg8 (int reg){
         union i2c_smbus_data data;

        if (i2c_smbus_access (I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data))
        {
            error = true ;
            return 0x0;
        }
        else
            return data.byte & 0xFF ;
    }

    /**
     * @brief Read 16 bit register
     * 
     * @param reg 
     * @return unsigned short 
     */
    unsigned short i2c::ReadReg16 (int reg){
        union i2c_smbus_data data;

        if (i2c_smbus_access (I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data))
        {
            error = true ;
            return 0x0;
        }
        else
            return data.word & 0xFFFF ;
    }

    /**
     * @brief Write func
     * 
     * @param data 
     * @return unsigned char 
     */
    unsigned char i2c::Write (int data){
        return i2c_smbus_access (I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, NULL) ;

    }

    /**
     * @brief Write into a 8 bit register
     * 
     * @param reg 
     * @param value 
     * @return unsigned char 
     */
    unsigned char i2c::WriteReg8 (int reg, int value){
        union i2c_smbus_data data ;

        data.byte = value ;
        return i2c_smbus_access (I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data) ;
    }

    /**
     * @brief Write into a 16 bit register
     * 
     * @param reg 
     * @param value 
     * @return unsigned short 
     */
    unsigned short i2c::WriteReg16 (int reg, int value){
        union i2c_smbus_data data ;

        data.word = value ;
        return i2c_smbus_access (I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data) ;

    }

    /**
     * @brief Write Block Data
     * 
     * @param reg 
     * @param length 
     * @param values 
     * @return int 
     */
    int i2c::WriteBlockData (int reg, int length, int *values){
	union i2c_smbus_data data ;
        int i;
        if (length > 32)
	    length = 32;
        for (i = 1; i <= length; i++)
	    data.block[i] = values[i-1];
	data.block[0] = length;
        return i2c_smbus_access (I2C_SMBUS_WRITE, reg, I2C_SMBUS_I2C_BLOCK_BROKEN , &data) ;
    }

    /**
     * @brief Read Block Data
     * 
     * @param reg 
     * @param length 
     * @param values 
     * @return int 
     */
    int i2c::ReadBlockData (int reg, int length, int *values){
         union i2c_smbus_data data;
         int i;

         if (length > 32)
                 length = 32;
         data.block[0] = length;
         if (i2c_smbus_access(I2C_SMBUS_READ, reg, length == 32 ? I2C_SMBUS_I2C_BLOCK_BROKEN :
                               I2C_SMBUS_I2C_BLOCK_DATA,&data)){
	     error = true;
             return -1;
	 }
         else {
                 for (i = 1; i <= data.block[0]; i++)
                         values[i-1] = data.block[i];
                 return data.block[0];
         }
 }
