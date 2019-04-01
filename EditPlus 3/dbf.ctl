#TITLE=       DBF数据库类
#INFO
EditPlus Cliptext Library v1.0 written by ES-Computing.
This file is provided as a sample Clitext library of EditPlus.
#SORT=n
 
#T=_Select   选择工作区
_Select(
#T=_Go       定位记录	
_Go( 
#T=_Skip     相对移动记录
_Skip( 
#T=_Use      打开数据库
_Use(
#T=_Recno    获取当前记录	
_Recno()
#T=_Reccount 获取当前工作区总记录
_Reccount()
#T=_Bof      是否到第一条记录	
_Bof()
#T=_Eof      是否到最后一条记录	
_Eof();
#T=_App      追加一条空记录
_App();
#T=_Del      逻辑删除当前记录
_Del();
#T=_Pack     物理删除记录
_Pack();
#T=_Zap      清空所有记录	 
_Zap();
#T=_Replace  更新记录
_Replace(
#T=_ReadField读字段
_ReadField( 
 #T=MRD_SBH C  30
MRD_SBH
#T=MRD_YBH C  30
MRD_YBH
#T=MRD_XM C  50
MRD_XM
#T=MRD_DZ C  50
MRD_DZ
#T=MRD_ZDDS N  19
MRD_ZDDS
#T=MRD_SYBS N  19
MRD_SYBS
#T=MRD_BYBS N  19
MRD_BYBS
#T=MRD_SYYSL N  19
MRD_SYYSL
#T=MRD_CBBZ C  1
MRD_CBBZ
#T=MRD_HBBZ C  1
MRD_HBBZ
#T=MRD_SYQF N  19
MRD_SYQF
#T=MRD_ZNJ N  19
MRD_ZNJ
#T=MRD_PWF N  19
MRD_PWF
#T=MRD_JSJT N  10
MRD_JSJT
#T=MRD_SFDJ1 N  19
MRD_SFDJ1
#T=MRD_SFDJ2 N  10
MRD_SFDJ2
#T=MRD_SFDJ3 N  10
MRD_SFDJ3
#T=MRD_PWFDJ N  19
MRD_PWFDJ
#T=MRD_JTSLJS1 N  10
MRD_JTSLJS1
#T=MRD_JTSLJS2 N  10
MRD_JTSLJS2
#T=MRD_JTSLJS3 N  10
MRD_JTSLJS3
#T=MRD_CBRQ C  10
MRD_CBRQ
#T=MRD_CBY C  10
MRD_CBY
#T=MRD_SSBH C  10
MRD_SSBH
#T=READ_READID C  10
READ_READID
#T=READ_BBH C  12
READ_BBH
#T=READ_BCBSX N  16
READ_BCBSX
#T=READ_BSB_ID C  15
READ_BSB_ID
#T=READ_BHM C  30
READ_BHM
#T=READ_BSBKJ N  16
READ_BSBKJ
#T=READ_BYSLX C  4
READ_BYSLX
#T=READ_BDZ C  30
READ_BDZ
#T=READ_BDH C  30
READ_BDH
#T=READ_BSQBD N  16
READ_BSQBD
#T=READ_BBQBD N  16
READ_BBQBD
#T=READ_BSQDS N  16
READ_BSQDS
#T=READ_BYSDS N  16
READ_BYSDS
#T=READ_BCBRQ C  8
READ_BCBRQ
#T=READ_BBZ C  1
READ_BBZ
#T=READ_BJS N  16
READ_BJS
#T=READ_MRBOOKID C  10
READ_MRBOOKID
#T=READ_MTYPE C  1
READ_MTYPE
#