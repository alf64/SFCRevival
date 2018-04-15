/*
 * comm.h
 *
 *  Created on: 15 kwi 2018
 *      Author: alf64
 */

#ifndef COMM_COMM_H_
#define COMM_COMM_H_

/*
 * Communicator is for communicating with the other
 * device (PC) via UART interface (as COM port).
 */


/*
 * This defines a maximum receive buffer, which stands
 * for maximum message (in bytes) that can be obtained (received)
 * via UART.
 */
#define COMM_RECEIVE_BUFFER_SIZE 16

typedef enum{
    COMM_SUCCESS,
    COMM_FAILED,
    COMM_RECEIVE_BUFF_EMPTY,
    COMM_RECEIVE_MSG_NOT_READY
}comm_status_t;


/*
 * Default msg for Communication
 */
static const char defaultmsg[] PROGMEM = "Comm initialization... OK";

/*
 * Inits the Communicator via UART interface.
 *
 * @details
 * This function initializes the communication mechanism via UART and also
 * sends a message upon successful initialization.
 *
 * @param baud_rate A desired baud rate (bps).
 * @param f_cpu A clock frequency of the CPU.
 * @param init_msg A pointer to a message which will be send right after successful init.
 * If NULL is passed as init_msg, function will send a defaultmsg[].
 * @param init_msg_size A size (in bytes) of the init message.
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to init the communicator.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommInit(
        uint32_t baud_rate,
        uint32_t f_cpu,
        const char* init_msg,
        uint8_t init_msg_size);

/*
 * Sends message via UART interface.
 *
 * @param msg A pointer to a message.
 * @param msg_size A size (in bytes) of the message.
 *
 * @attention
 * This function will only work properly if the CommInit() was performed before.
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to init the communicator.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommSendMsg(
        const char* msg,
        uint8_t msg_size);

/*
 * Same as CommSendMsg, but this one sends message that
 * is stored in program memory.
 */
comm_status_t CommSendMsgFromFlash(
        const char* PROGMEM msg,
        uint8_t msg_size);

/*
 * Gets a message from receive buffer.
 *
 * @param msg_size An (expected) size (in bytes) of the message to read.
 * @param msg A pointer where the message shall be copied to.
 *
 * @attention
 * This function will only work properly if the CommInit() was performed before.
 * The receive buffer will be totally flushed (restarted) upon a successful read
 * by this function (COMM_SUCCESS).
 *
 * @returns comm_status_t.
 * @retval COMM_SUCCESS Means the function succeeded to get a message of a given msg_size.
 * @retval COMM_RECEIVE_MSG_NOT_READY Means that something is in receive buffer,
 * but it is not yet a complete message.
 * @retval COMM_RECEIVE_BUFF_EMPTY Means the receive buffer is still empty.
 * @retval COMM_FAILED Means function failed. Probably unsupported arguments were given.
 */
comm_status_t CommGetMsg(
        uint8_t msg_size,
        char* msg);

#endif /* COMM_COMM_H_ */
