/*
 * Header: mqueue.h
 * 
 * Provides wrapper functions around POSIX system calls 
 * allowing the creation, opening
 * and disposal of message queues
 * 
 * 
 * Copyright 2016 Roger Miret, INSA de Lyon, TC department
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 * 
 */ 


#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int create_mqueue(int key) {
	return msgget(key, IPC_CREAT | 0666);
}

int open_mqueue(int key)
{
	return msgget(key, 0666) ;
}

int remove_mqueue(int id) 
{
	return msgctl(id, IPC_RMID, 0) ;
}

int send_mqueue(int id, int msg, int length) {
	return msgsnd(id, msg, length, IPC_NOWAIT);
}

int rcv_mqueue(int id, int msg, int length) {
	return msgrcv(id, msg, length, 1, 1);
}

#endif

