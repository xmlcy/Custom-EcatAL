/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/** \file
 * \brief
 * Headerfile for customecatal.c
 */

#ifndef _CUSTOM_ECAT_AL
#define _CUSTOM_ECAT_AL

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define DEBUG_ON 0
#if DEBUG_ON
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif

/**
  * @brief  ethercat initialization
  * @param  ifname: ifname
  */
void EthercatInit(char *ifname);

/**
  * @brief  ethercat processdata loop
  */
void EthercatProcessdataLoop(void);

void setOutputByte(uint16_t slave_no, uint8_t module_index, uint8_t value);
void setOutputUint16(uint16_t slave_no, uint8_t module_index, uint16_t value);
void setOutputInt16(uint16_t slave_no, uint8_t module_index, int16_t value);
uint8_t getInputByte(uint16_t slave_no, uint8_t module_index);
uint16_t getInputUint16(uint16_t slave_no, uint8_t module_index);
int16_t getInputInt16(uint16_t slave_no, uint8_t module_index);

/**
  * @brief  get 1 int32 of EtherCAT input
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @retval int32 value
  */
int32_t getInputInt32(uint16_t slave_no, uint8_t module_index);

/**
  * @brief  set 1 int32 of EtherCAT output
  * @param  slave_no: number of slave
  * @param  module_index: index of module in frame
  * @param  value: value of int32
  * @retval None
  */
void setOutputInt32(uint16_t slave_no, uint8_t module_index, int32_t value);

/**
  * @brief  get bytes of slave input
  * @param  slave_no: number of slave
  * @retval input bytes
  */
uint32_t getIbytes(uint16_t slave_no);

/**
  * @brief  get bytes of slave output
  * @param  slave_no: number of slave
  * @retval output bytes
  */
uint32_t getObytes(uint16_t slave_no);

/**
  * @brief  get count of EtherCAT slave
  * @retval slave count
  */
int getSlaveCount(void);


#ifdef __cplusplus
}
#endif

#endif /* _CUSTOM_ECAT_AL */
