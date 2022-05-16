/****************************
*							*
*	stdafx.h				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/16	*
*****************************/

#pragma once

/* Header */
#include <iostream>

/* Custom Header */
#include "Client.h"
#include "Player.h"
#include "GameProcess.h"

#define CLIENT Client::GetInstance()
#define GAMEPROCESS GameProcess::GetInstance()