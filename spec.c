	// brk
	{	0x0, ADDR_IMPL},
	// ora
	{	0x1, ADDR_X_IND},
	{	0x5, ADDR_ZPG},
	{	0x9, ADDR_IMM},
	{	0xd, ADDR_ABS},
	{	0x11, ADDR_IND_Y},
	{	0x15, ADDR_ZPG_X},
	{	0x19, ADDR_ABS_Y},
	{	0x1d, ADDR_ABS_X},
	// asl
	{	0x6, ADDR_ZPG},
	{	0xa, ADDR_A},
	{	0xe, ADDR_ABS},
	{	0x16, ADDR_ZPG_X},
	{	0x1e, ADDR_ABS_X},
	// php
	{	0x8, ADDR_IMPL},
	// bpl
	{	0x10, ADDR_REL},
	// clc
	{	0x18, ADDR_IMPL},
	// jsr
	{	0x20, ADDR_ABS},
	// and
	{	0x21, ADDR_X_IND},
	{	0x25, ADDR_ZPG},
	{	0x29, ADDR_IMM},
	{	0x2d, ADDR_ABS},
	{	0x31, ADDR_IND_Y},
	{	0x35, ADDR_ZPG_X},
	{	0x39, ADDR_ABS_Y},
	{	0x3d, ADDR_ABS_X},
	// bit
	{	0x24, ADDR_ZPG},
	{	0x2c, ADDR_ABS},
	// rol
	{	0x26, ADDR_ZPG},
	{	0x2a, ADDR_A},
	{	0x2e, ADDR_ABS},
	{	0x36, ADDR_ZPG_X},
	{	0x3e, ADDR_ABS_X},
	// plp
	{	0x28, ADDR_IMPL},
	// bmi
	{	0x30, ADDR_REL},
	// sec
	{	0x38, ADDR_IMPL},
	// rti
	{	0x40, ADDR_IMPL},
	// eor
	{	0x41, ADDR_X_IND},
	{	0x45, ADDR_ZPG},
	{	0x49, ADDR_IMM},
	{	0x4d, ADDR_ABS},
	{	0x51, ADDR_IND_Y},
	{	0x55, ADDR_ZPG_X},
	{	0x59, ADDR_ABS_Y},
	{	0x5d, ADDR_ABS_X},
	// lsr
	{	0x46, ADDR_ZPG},
	{	0x4a, ADDR_A},
	{	0x4e, ADDR_ABS},
	{	0x56, ADDR_ZPG_X},
	{	0x5e, ADDR_ABS_X},
	// pha
	{	0x48, ADDR_IMPL},
	// jmp
	{	0x4c, ADDR_ABS},
	{	0x6c, ADDR_IND},
	// bvc
	{	0x50, ADDR_REL},
	// cli
	{	0x58, ADDR_IMPL},
	// rts
	{	0x60, ADDR_IMPL},
	// adc
	{	0x61, ADDR_X_IND},
	{	0x65, ADDR_ZPG},
	{	0x69, ADDR_IMM},
	{	0x6d, ADDR_ABS},
	{	0x71, ADDR_IND_Y},
	{	0x75, ADDR_ZPG_X},
	{	0x79, ADDR_ABS_Y},
	{	0x7d, ADDR_ABS_X},
	// ror
	{	0x66, ADDR_ZPG},
	{	0x6a, ADDR_A},
	{	0x6e, ADDR_ABS},
	{	0x76, ADDR_ZPG_X},
	{	0x7e, ADDR_ABS_X},
	// pla
	{	0x68, ADDR_IMPL},
	// bvs
	{	0x70, ADDR_REL},
	// sei
	{	0x78, ADDR_IMPL},
	// sta
	{	0x81, ADDR_X_IND},
	{	0x85, ADDR_ZPG},
	{	0x8d, ADDR_ABS},
	{	0x91, ADDR_IND_Y},
	{	0x95, ADDR_ZPG_X},
	{	0x99, ADDR_ABS_Y},
	{	0x9d, ADDR_ABS_X},
	// sty
	{	0x84, ADDR_ZPG},
	{	0x8c, ADDR_ABS},
	{	0x94, ADDR_ZPG_X},
	// stx
	{	0x86, ADDR_ZPG},
	{	0x8e, ADDR_ABS},
	{	0x96, ADDR_ZPG_Y},
	// dey
	{	0x88, ADDR_IMPL},
	// txa
	{	0x8a, ADDR_IMPL},
	// bcc
	{	0x90, ADDR_REL},
	// tya
	{	0x98, ADDR_IMPL},
	// txs
	{	0x9a, ADDR_IMPL},
	// ldy
	{	0xa0, ADDR_IMM},
	{	0xa4, ADDR_ZPG},
	{	0xac, ADDR_ABS},
	{	0xb4, ADDR_ZPG_X},
	{	0xbc, ADDR_ABS_X},
	// lda
	{	0xa1, ADDR_X_IND},
	{	0xa5, ADDR_ZPG},
	{	0xa9, ADDR_IMM},
	{	0xad, ADDR_ABS},
	{	0xb1, ADDR_IND_Y},
	{	0xb5, ADDR_ZPG_X},
	{	0xb9, ADDR_ABS_Y},
	{	0xbd, ADDR_ABS_X},
	// ldx
	{	0xa2, ADDR_IMM},
	{	0xa6, ADDR_ZPG},
	{	0xae, ADDR_ABS},
	{	0xb6, ADDR_ZPG_Y},
	{	0xbe, ADDR_ABS_Y},
	// tay
	{	0xa8, ADDR_IMPL},
	// tax
	{	0xaa, ADDR_IMPL},
	// bcs
	{	0xb0, ADDR_REL},
	// clv
	{	0xb8, ADDR_IMPL},
	// tsx
	{	0xba, ADDR_IMPL},
	// cpy
	{	0xc0, ADDR_IMM},
	{	0xc4, ADDR_ZPG},
	{	0xcc, ADDR_ABS},
	// cmp
	{	0xc1, ADDR_X_IND},
	{	0xc5, ADDR_ZPG},
	{	0xc9, ADDR_IMM},
	{	0xcd, ADDR_ABS},
	{	0xd1, ADDR_IND_Y},
	{	0xd5, ADDR_ZPG_X},
	{	0xd9, ADDR_ABS_Y},
	{	0xdd, ADDR_ABS_X},
	// dec
	{	0xc6, ADDR_ZPG},
	{	0xce, ADDR_ABS},
	{	0xd6, ADDR_ZPG_X},
	{	0xde, ADDR_ABS_X},
	// iny
	{	0xc8, ADDR_IMPL},
	// dex
	{	0xca, ADDR_IMPL},
	// bne
	{	0xd0, ADDR_REL},
	// cld
	{	0xd8, ADDR_IMPL},
	// cpx
	{	0xe0, ADDR_IMM},
	{	0xe4, ADDR_ZPG},
	{	0xec, ADDR_ABS},
	// sbc
	{	0xe1, ADDR_X_IND},
	{	0xe5, ADDR_ZPG},
	{	0xe9, ADDR_IMM},
	{	0xed, ADDR_ABS},
	{	0xf1, ADDR_IND_Y},
	{	0xf5, ADDR_ZPG_X},
	{	0xf9, ADDR_ABS_Y},
	{	0xfd, ADDR_ABS_X},
	// inc
	{	0xe6, ADDR_ZPG},
	{	0xee, ADDR_ABS},
	{	0xf6, ADDR_ZPG_X},
	{	0xfe, ADDR_ABS_X},
	// inx
	{	0xe8, ADDR_IMPL},
	// nop
	{	0xea, ADDR_IMPL},
	// beq
	{	0xf0, ADDR_REL},
	// sed
	{	0xf8, ADDR_IMPL},
