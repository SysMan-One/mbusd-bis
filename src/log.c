/*
 * OpenMODBUS/TCP to RS-232/485 MODBUS RTU gateway
 *
 * log.c - debug logging facility
 *
 * Copyright (c) 2002-2003, 2013, Victor Antonovich (v.antonovich@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: log.c,v 1.6 2015/02/25 10:33:57 kapyar Exp $
 */

#include	"utility_routines.h"
#include	"log.h"


#ifdef LOG
#include "cfg.h"

/* log file full name */
char logfullname[INTBUFSIZE + 1];

int log_init(char *logname)
{

	if ( 1 & __util$deflog (logname, NULL) )
		return	RC_OK;

	return RC_ERR;
}



/* Put message with format FMT with errorlevel LEVEL to log file */
void __logw (
	const char	*a_func,
		int	a_line,
		int	level,
	const char	*a_fmt,
		...
		)
{
char	l_msg[8192], *l_cp;
int	l_len;
va_list a_args;

	switch (level)									/* Translate SPDK's log levels to the severity status */
		{
		case	0:	return;
		case	1:		level = STS$K_ERROR;	break;
		case	2:		level = STS$K_WARN;	break;
		case	3:		level = STS$K_SUCCESS;	break;
		case	4:		level = STS$K_INFO;	break;
		case	5:		level = STS$K_INFO;	break;
		default:		level = STS$K_INFO;	break;
		}

	va_start (a_args, a_fmt);
	l_len = vsnprintf(l_msg, sizeof(l_msg), a_fmt, a_args);				/* Format SPDK's message into the buffer */
	va_end (a_args);

	for (l_cp = ((l_msg + l_len) - 1); l_len; l_len--, l_cp--)			/* Remove extra CR, LF */
		{
		if ( !((*l_cp == '\r') || (*l_cp == '\n')) )
			break;
		}

	l_msg[l_len] = '\0';								/* Make ASCIZ */


	__util$logd( "MBUSD", level, l_msg, "MBUSD", a_func, a_line);
}
#endif
