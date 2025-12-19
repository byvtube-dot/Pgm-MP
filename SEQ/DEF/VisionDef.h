#pragma once
enum VisionList
{
	VI_LABEL_QC							= 0,
	VI_LABEL_PRS						= 1,
	VI_IN_PK							= 2,
	VI_IN_PK_BAND						= 3,
	VI_RT_COBOT							= 4,
	VI_MBB_QC							= 5,

	VI_MAX,
};

enum ViCount
{
	INLET_ALIGN_CNT						= 2,
};

enum ViMode
{
	// In Picker Band
	MODE_IN_PK_BANDING_SIDE				= 0,	// OK,NG
	MODE_IN_PK_BANDING_CENTER			= 1,	// OK,NG

	// In Picker 
	MODE_IN_PK_BTM_FOAM_TRAY			= 0,	// OK,NG	// PP Banding 후 Dessicant & HIC 유무 검사	// Banding 후 부자재 유무, DESICCANT 터짐, HIC 인쇄불량, HIC 변색
	MODE_IN_PK_BTM_FOAM					= 1,	// OK,NG	// FORM PAD 덮기 전 이물질 오입 검사			// 정상 자재 외 이물질 오입 이상감지		// FOD:Foreign Object Inclusion
	MODE_IN_PK_FOAM_LABEL_ATTACHED		= 2,	// OK,NG	// LABEL 부착 결과 검사						// 바코드 인쇄 상태, 인쇄 위치, 바코드 인식, 면적, 길이

	// Rt Cobot
	MODE_RT_COBOT_MBB_MISPLACED			= 0,	// OK,NG	// MBB PAD 혼입/뒤집힘 검사					// MBB 공급방향 및 혼입 검사
	MODE_RT_COBOT_MBB_LABEL_ATTACHED	= 1,	// OK,NG	// LABEL 부착 결과 검사						// 바코드 인쇄 상태, 인쇄 위치, 바코드 인식, 면적, 길이

	// Label Prs
	MODE_LABEL_PRS_LABEL_EDGE_LT_TOP	= 0,	// OK,NG	// LABEL EDGE LEFT TOP
	MODE_LABEL_PRS_1POINT_THETA			= 1,	// OK,NG	// 1POINT THETA
	MODE_LABEL_PRS_1POINT				= 2,	// OK,NG	// 1POINT CALIBRATION

	// Label Qc
	MODE_LABEL_QC_PRINTED				= 0,	// OK,NG	// LABEL 출력(인쇄) 검사						// 바코드 인쇄 상태, 인쇄 위치, 바코드 인식, 면적, 길이

	// Mbb Sealing Qc
	MODE_MBB_QC_SEALED_TOP				= 0,	// OK,NG	// MBB Sealing 후 상단면 검사					// MBB Sealing 부 밀림 검사, MBB 상단면 damage검사
	MODE_MBB_QC_SEALED_MID				= 1,	// OK,NG	// MBB Sealing 후 상단면 검사					// MBB Sealing 부 밀림 검사, MBB 상단면 damage검사
	MODE_MBB_QC_SEALED_BTM				= 2,	// OK,NG	// MBB Sealing 후 상단면 검사					// MBB Sealing 부 밀림 검사, MBB 상단면 damage검사
	MODE_MBB_QC_SEALED_MARK				= 3,	// OK,NG	// MBB Sealing 후 상단면 검사					// MBB Sealing 부 밀림 검사, MBB 상단면 damage검사

	ModeMax								= 16,
};