//------------------------------------------------------------------------------
//
// File Name:	Trace.c
// Author(s):	Doug Schilling (dschilling), Jeremy Kings (j.kings)
// Project:		MyGame
// Course:		CS230S17
//
// Copyright © 2017 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdarg.h>
#include "Trace.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Tracing/Logging module.
void Trace::Init()
{
	// TODO: Open "trace.log" for writing (text mode).

	// Error handling:
	// https://msdn.microsoft.com/en-us/library/9t0e6085.aspx
	traceFile.open("trace.log");

	_set_errno(0);
	if (traceFile.is_open())
	{
		traceStream.set_rdbuf(traceFile.rdbuf());
		fileOpened = true;
	}
	else
	{
		char errMsg[1024];
		strerror_s(errMsg, _countof(errMsg), errno);
		std::cout << "Trace::Init Error: File open failed: " << errMsg << std::endl;
		fileOpened = false;
	}
}

// Output a message to the Tracing/Logging file.
std::ostream& Trace::GetStream()
{
	return traceStream;
}

// Shutdown the Tracing/Logging module.
void Trace::Shutdown()
{
	/// TODO: Close "trace.log" if it has been opened successfully.
	if (fileOpened)
	{
		traceFile.close();
	}
}

// Retrieve the instance of the Trace singleton
Trace& Trace::GetInstance()
{
	static Trace instance;
	return instance;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Constructor is private to prevent accidental instantiation
Trace::Trace()
	: traceStream(std::cout.rdbuf()), fileOpened(false)
{
}
