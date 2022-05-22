/****************************
*							*
*	Protocol.h				*
*							*
*	Created : 2022/05/22	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once

enum class Protocol
{
	NONE,

	PRINT = 10,

	ECHO = 100,

	BROADCAST = 1000,
};