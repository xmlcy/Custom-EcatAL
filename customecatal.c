/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/** \file
 * \brief
 * EthercatInit() and some useful fuc
 */

#include "customecatal.h"
#include "ethercat.h"
#include <stdio.h>

char IOmap[4096];
volatile int wkc;

/**
  * @brief  ethercat initialization
  * @param  ifname: ifname
  */
void EthercatInit(char *ifname)
{
    int oloop, iloop, chk;

    /* initialise SOEM, bind socket to ifname */
    if (ec_init(ifname)) {
        DEBUG("ec_init on %s succeeded.\n", ifname);

        /* find and auto-config slaves */
        if (ec_config_init(FALSE) > 0) {
            DEBUG("%d slaves found and configured.\n", ec_slavecount);

            /*Mapping data*/
            ec_config_map(&IOmap);
            ec_configdc();

            for (int i = 0; i <= ec_slavecount; i++) {
                DEBUG("Slave %d: %s ", i, ec_slave[i].name);
                DEBUG("Input: %d bytes ", ec_slave[i].Ibytes);
                DEBUG("Output: %d bytes\n", ec_slave[i].Obytes);
            }

            DEBUG("Slaves mapped, state to SAFE_OP.\n");
            /* wait for all slaves to reach SAFE_OP state */
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);

            oloop = ec_slave[0].Obytes;
            if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
            if (oloop > 8) oloop = 8;
            iloop = ec_slave[0].Ibytes;
            if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
            if (iloop > 8) iloop = 8;

            // DEBUG("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

            ec_slave[0].state = EC_STATE_OPERATIONAL;
            /* send one valid process data to make outputs in slaves happy*/
            ec_send_processdata();
            ec_receive_processdata(EC_TIMEOUTRET);
            /* request OP state for all slaves */
            ec_writestate(0);
            /* wait for all slaves to reach OP state */
            do {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            } while (ec_slave[0].state != EC_STATE_OPERATIONAL);

        } else {
            DEBUG("No slaves found!\n");
        }
    } else {
        DEBUG("No socket connection on %s\nExecute as root\n", ifname);
    }
}

/**
  * @brief  ethercat processdata loop
  */
void EthercatProcessdataLoop(void)
{
    ec_send_processdata();
    ec_receive_processdata(2000);
}

/**
  * @brief  get 1 int32 of EtherCAT input
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @retval int32 value
  */
int32_t getInputInt32(uint16_t slave_no, uint8_t module_index)
{
    int32_t return_value;
    uint8_t *data_ptr;
    /* Get the IO map pointer from the ec_slave struct */
    data_ptr = ec_slave[slave_no].inputs;
    /* Move pointer to correct module index */
    data_ptr += module_index * 4;
    /* Read value byte by byte since all targets can't handle misaligned
    * addresses
    */
    return_value = *data_ptr++;
    return_value += (*data_ptr++ << 8);
    return_value += (*data_ptr++ << 16);
    return_value += (*data_ptr++ << 24);

    return return_value;
}

/**
  * @brief  set 1 int32 of EtherCAT output
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @param  value: value of int32
  * @retval None
  */
void setOutputInt32(uint16_t slave_no, uint8_t module_index, int32_t value)
{
    uint8_t *data_ptr;
    /* Get the IO map pointer from the ec_slave struct */
    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct module index */
    data_ptr += module_index * 4;
    /* Read value byte by byte since all targets can handle misaligned
    * addresses
    */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
    *data_ptr++ = (value >> 16) & 0xFF;
    *data_ptr++ = (value >> 24) & 0xFF;
}

/**
  * @brief  set 1 byte of EtherCAT output
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @param  value: value of byte
  * @retval None
  */
void setOutputByte(uint16_t slave_no, uint8_t module_index, uint8_t value)
{
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct module index*/
    data_ptr += module_index;
    /* Read value byte by byte since all targets can't handle misaligned
    addresses
    */
    *data_ptr++ = value;
    }

/**
  * @brief  set 1 uint16 of EtherCAT output
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @param  value: value of uint16
  * @retval None
  */
void setOutputUint16(uint16_t slave_no, uint8_t module_index, uint16_t value)
{
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct module index*/
    data_ptr += module_index;
    /* Read value byte by byte since all targets can't handle misaligned
    addresses
    */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
}

/**
  * @brief  set 1 int16 of EtherCAT output
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @param  value: value of int16
  * @retval None
  */
void setOutputInt16(uint16_t slave_no, uint8_t module_index, int16_t value)
{
    uint8_t *data_ptr;

    data_ptr = ec_slave[slave_no].outputs;
    /* Move pointer to correct module index*/
    data_ptr += module_index;
    /* Read value byte by byte since all targets can't handle misaligned addresses
    */
    *data_ptr++ = (value >> 0) & 0xFF;
    *data_ptr++ = (value >> 8) & 0xFF;
}

/**
  * @brief  get 1 byte of EtherCAT input
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @retval byte value
  */
uint8_t getInputByte(uint16_t slave_no, uint8_t module_index)
{
    uint8_t *u8;
    uint8_t byte_in;

    u8 = (uint8_t*) (ec_slave[slave_no].inputs + module_index);
    byte_in = *u8;

    return byte_in;
}

/**
  * @brief  get 1 uint16 of EtherCAT input
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @retval uint16 value
  */
uint16_t getInputUint16(uint16_t slave_no, uint8_t module_index)
{
    uint16_t *u16;
    uint16_t uint16_in;

    u16 = (uint16_t*) (ec_slave[slave_no].inputs + module_index);
    uint16_in = *u16;

    return uint16_in;
}

/**
  * @brief  get 1 int16 of EtherCAT input
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @retval int16 value
  */
int16_t getInputInt16(uint16_t slave_no, uint8_t module_index)
{
    int16_t *u16;
    int16_t int16_in;

    u16 = (int16_t*) (ec_slave[slave_no].inputs + module_index);
    int16_in = *u16;

    return int16_in;
}

/**
  * @brief  get bytes of slave input
  * @param  slave_no: number of slave
  * @retval input bytes
  */
uint32_t getIbytes(uint16_t slave_no)
{
    return ec_slave[slave_no].Ibytes;
}

/**
  * @brief  get bytes of slave output
  * @param  slave_no: number of slave
  * @retval output bytes
  */
uint32_t getObytes(uint16_t slave_no)
{
    return ec_slave[slave_no].Obytes;
}

/**
  * @brief  get count of EtherCAT slave
  * @retval slave count
  */
int getSlaveCount(void)
{
    return ec_slavecount;
}
