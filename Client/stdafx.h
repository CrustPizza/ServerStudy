/****************************
*							*
*	stdafx.h				*
*							*
*	Created : 2022/05/16	*
*	Updated : 2022/05/22	*
*****************************/

#pragma once

/* Header */
#include <iostream>

/* Custom Header */
#include "Client.h"
#include "Player.h"
#include "GameProcess.h"
#include "PlayerManager.h"

#define CLIENT Client::GetInstance()
#define GAMEPROCESS GameProcess::GetInstance()