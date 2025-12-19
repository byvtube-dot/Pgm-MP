#pragma once

enum LogList
{
	logMT,
	logIO,
	logDM,
	logMC,
	logSYS,
	logSEQ,
	logGEM,
	logMMI,
	logLAS,
	logLASERIO,
	logROBOT,
	logMAX,
};

enum SLogList
{
	slogInPk,
	slogFoamRail,
	slogTrayShuttle,
	slogFoamSendElev,
	slogFoamRecvElev,
	slogFoamPartialElev,
	slogFoamConv,
	slogFoamPk,
	slogFoamLtCst,
	slogFoamRtCst,
	slogMbbShuttle,
	slogMbbQc,
	slogBtmFoldConv,
	slogFoldPk,
	slogLtLabelFeeder,
	slogRtLabelFeeder,
	slogLabelLaser,
	slogLtLabelShuttle,
	slogRtLabelShuttle,
	slogMbbLtCst,
	slogMbbRtCst,
	slogLtCobot,
	slogRtCobot,
	slogMAX,
};