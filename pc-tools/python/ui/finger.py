# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Jun 17 2015)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import platform
import sys
import wx
import wx.xrc
import wx.grid
import os
import ctypes
import re
import subprocess 
import numpy as np
import numpy
import datetime
import uartcmd
import threading
#import serial
import struct

#from compiler.ast import flatten

#array=np.array(a) a is a list

#numpy.savetxt("result.txt", numpy_data)

###########################################################################
## Class MyFrame1
###########################################################################
class warning_window ( wx.Dialog ):
  def __init__( self, parent, msg="Warning" ):
    wx.Dialog.__init__ ( self, parent, id = wx.ID_ANY, title = u"Warning!", pos = wx.DefaultPosition, size = wx.Size( 453,250 ), style = wx.DEFAULT_DIALOG_STYLE )
    
    self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
    
    bSizer7 = wx.BoxSizer( wx.VERTICAL )
    
    bSizer8 = wx.BoxSizer( wx.HORIZONTAL )
    
    self.m_staticText7 = wx.StaticText( self, wx.ID_ANY, msg, wx.DefaultPosition, wx.DefaultSize, 0 )
    self.m_staticText7.Wrap( -1 )
    bSizer8.Add( self.m_staticText7, 1, wx.ALIGN_CENTER_HORIZONTAL|wx.ALL|wx.EXPAND, 5 )
    
    
    bSizer7.Add( bSizer8, 3, wx.EXPAND, 5 )
    
    bSizer9 = wx.BoxSizer( wx.VERTICAL )
    
    self.m_button6 = wx.Button( self, wx.ID_OK, u"确定", wx.DefaultPosition, wx.DefaultSize, 0 )
    bSizer9.Add( self.m_button6, 0, wx.ALIGN_CENTER, 5 )
    
    
    bSizer7.Add( bSizer9, 1, wx.EXPAND, 5 )
    
    
    self.SetSizer( bSizer7 )
    self.Layout()
    
    self.Centre( wx.BOTH )
   # LoadLibrary = ctypes.cdll.LoadLibrary
   # self.serial = LoadLirary("./libabc.so")
  
  def __del__( self ):
    pass

class MyFrame1 ( wx.Frame ):
	filename = None
	serialobj = None
	regfile = "finger.reg"
	regs = []
	reg_values = []
	popen = None
	filename = ""
	cmd = None
	
	reg = [
        [0xfe, 0x00],
        [0x01, 0xfa],
        [0x02, 0x70],
        [0x0f, 0x01],
        [0x03, 0x00],
        [0x04, 0x10],
        [0xe2, 0x00],  #anti-flicker step [11:8]
        [0xe3, 0x64],  #anti-flicker step [7:0]
        [0xe4, 0x02],  #exp level 1  16.67fps
        [0xe5, 0x58],
        [0xe6, 0x03],  #exp level 2  12.5fps
        [0xe7, 0x20],
        [0xe8, 0x04],  # 0x02},  //exp level 3  8.33fps
        [0xe9, 0xb0],  # 0x58},
        [0xea, 0x09],  #exp level 4  4.00fps
        [0xeb, 0xc4],
        #  200*240
        #{0x05, 0x00], # row_start_high
        #{0x06, 0x8c], # row_start_low
        #{0x07, 0x00], # col_start_high
        #{0x08, 0xc8], # col_start_low
        #{0x09, 0x00], #[8]cis_win_height  208
        #{0x0a, 0xf8], #[7:0]cis_win_height
        #{0x0b, 0x00], #[9:8]cis_win_width 248
        #{0x0c, 0xd0], #[7:0]cis_win_width

        #  240*200
        #[0x05, 0x00], # row_start_high
        #[0x06, 0xc8], # row_start_low     // 200
        #[0x07, 0x00], # col_start_high
        #[0x08, 0x8c], # col_start_low     // 140
        #[0x09, 0x00], #[8]cis_win_height   208
        #[0x0a, 0xd0], #[7:0]cis_win_height
        #[0x0b, 0x00], #[9:8]cis_win_width  248
        #[0x0c, 0xf8], #[7:0]cis_win_width

        # 480*100 原始640*480
        #[0x05, 0x00], # row_start_high
        #[0x06, 0x50], # row_start_low      //
        #[0x07, 0x00], # col_start_high
        #[0x08, 0xbe], # col_start_low      //
        #[0x09, 0x00], #[8]cis_win_height
        #[0x0a, 0x6c], #[7:0]cis_win_height //  0x64+8 配置后+8
        #[0x0b, 0x01], #[9:8]cis_win_width
        #[0x0c, 0xe8], #[7:0]cis_win_width  //  0x1e+8 配置后+8

        #   480*480
        #[0x05, 0x00], # row_start_high
        #[0x06, 0x00], # row_start_low
        #[0x07, 0x00], # col_start_high
    #[0x08, 0x00], # col_start_low
        #[0x09, 0x01], #[8]cis_win_height
        #[0x0a, 0xe8], #[7:0]cis_win_height
        #[0x0b, 0x01], #[9:8]cis_win_width
        #[0x0c, 0xe8], #[7:0]cis_win_width

        #   640*480
        #[0x05, 0x00], # row_start_high
        #[0x06, 0x00], # row_start_low
        #[0x07, 0x00], # col_start_high
        #[0x08, 0x00], # col_start_low
        #[0x09, 0x01], #[8]cis_win_height
        #[0x0a, 0xe8], #[7:0]cis_win_height
        #[0x0b, 0x02], #[9:8]cis_win_width
        #[0x0c, 0x88], #[7:0]cis_win_width

        #   360*360
        #[0x05, 0x00], # row_start_high
        #[0x06, 0x8c], # row_start_low
        #[0x07, 0x00], # col_start_high
        #[0x08, 0x3c], # col_start_low
        #[0x09, 0x01], #[8]cis_win_height
        #[0x0a, 0x70], #[7:0]cis_win_height
        #[0x0b, 0x01], #[9:8]cis_win_width
        #[0x0c, 0x70], #[7:0]cis_win_width

        #   400*300
        [0x05, 0x00], # row_start_high
        [0x06, 0x60], # row_start_low
        [0x07, 0x00], # col_start_high
        [0x08, 0x00], # col_start_low
        [0x09, 0x01], #[8]cis_win_height
        [0x0a, 0x34], #[7:0]cis_win_height
        [0x0b, 0x01], #[9:8]cis_win_width
        [0x0c, 0x96], #[7:0]cis_win_width

        #   240*240
        #[0x05, 0x00], # row_start_high
        #[0x06, 0xc8], # row_start_low
        #[0x07, 0x00], # col_start_high
        #[0x08, 0x78], # col_start_low
        #[0x09, 0x00], #[8]cis_win_height
        #[0x0a, 0xf8], #[7:0]cis_win_height
        #[0x0b, 0x00], #[9:8]cis_win_width
        #[0x0c, 0xf8], #[7:0]cis_win_width

        [0x0d, 0x02],  #vs_st
        [0x0e, 0x02],  #vs_et
        [0x10, 0x26],  #[7:4]restg_width, [3:0]sh_width
        [0x11, 0x0d],  #fd//[7:4]tx_width, [3:0]space width,*2
        [0x12, 0x2a],  #sh_delay
        [0x13, 0x00],  #[3:0] row_tail_width
        [0x14, 0x10],  #[7]hsync_always ,[6] NA,  [5:4] CFA sequence
        [0x15, 0x0a],  #[7:6]output_mode,,[5:4]restg_mode,[3:2]sdark_mode, [1]new exposure,[0]badframe_en
        [0x16, 0x05],  #[7:5]NA, [4]capture_ad_data_edge, [3:0]Number of A/D pipe stages 
        [0x17, 0x01],  #[7:6]analog_opa_r,[5]coltest_en, [4]ad_test_enable, 
        [0x18, 0x44],  #[7]NA,  [6:4]column gain ee, [3]NA, [2:0]column gain eo
        [0x19, 0x44],  #[7]NA,  [6:4]column gain oe, [3]NA, [2:0]column gain oo
        [0x1a, 0x2a],  #[3:2]colg_r column gain opa bias current, [1]clk_delay, [0] apwd
        [0x1b, 0x00],  #[7:2]NA, [1:0]BIN4 AND BIN2
        [0x1c, 0x49],  #c1//[7]hrst_enbale, [6:4]da_rsg, [3]tx high enable, [2]NA, [1:0]da18_r 
        [0x1d, 0x9a],  #08//[7]vref_en, [6:4]da_vef, [3]da25_en, [2]NA, [1:0]da25_r,set da25 voltage
        [0x1e, 0x61],  #60//[7]LP_MTD,[6:5]opa_r,ADC's operating current,  [4:2]NA, [1:0]sref
        [0x1f, 0x15],  #[7:6]NA, [5:4]sync_drv, [3:2]data_drv, [1:0]pclk_drv
        [0x20, 0xef],  #[7]bks[6]gamma[5]cc[4]ee[3]intp[2]dn[1]dd[0]lsc
        [0x21, 0xf8],  #[7]na[6]na[5]skin_ee[4]cbcr_hue_en[3]y_as_en[2]auto_gray_en[1]y_gamma_en[0]na
        [0x22, 0x00],  #[7]na [6]auto_dndd [5]auto_ee [4]auto_sa [3]na [2]abs [1]awb  [0]na
        [0x24, 0xb1],  #YCbYcr //  a2
        [0x25, 0x0f],
        [0x26, 0x03],  #02		// 07-16原始值01
        [0x2f, 0x01],  #debug mode3
        [0x30, 0xf7],  #blk mode [7]dark current mode:1 use exp rated dark ,0 use ndark row calculated  
        [0x31, 0x50],  #blk_value limit.64 low align to 11bits;8 for 256 range
        [0x32, 0x00],  #global offset
        [0x39, 0x04],  # exp_ate_darkc
        [0x3a, 0x20],  #{7:6}offset submode {5:0}offset ratio
        [0x3b, 0x20],  #{7:6}darkc submode {5:0}dark current ratio
        [0x3c, 0x00],  #manual g1 offset
        [0x3d, 0x00],  #manual r offset
        [0x3e, 0x00],  #manual b offset
        [0x3f, 0x00],  #manual g2 offset
        [0x50, 0x10],  #10  //global gain
        [0x53, 0x80],  #G 
        [0x54, 0x80],  #R channel gain
        [0x55, 0x80],  #B channel gain
        [0x56, 0x80],
        [0x8b, 0x20],  #r2
        [0x8c, 0x20],  #g2
        [0x8d, 0x20],  #b2
        [0x8e, 0x14],  #r4
        [0x8f, 0x10],  #g4
        [0x90, 0x14],  #b4
        [0x91, 0x3c],  #[7]singed4 [6:0]row_cneter
        [0x92, 0x50],  #col_center
        [0x5d, 0x12],  #decrease 1
        [0x5e, 0x1a],  #decrease 2
        [0x5f, 0x24],  #decrease 3
        [0x60, 0x07],  #[4]zero weight mode
        [0x61, 0x15],  #[7:6]na
        [0x62, 0x20],  #b base		//0x08
        [0x64, 0x03],  #[7:4]n base [3:0]c weight
        [0x66, 0xe8],  #dark_th ,bright_th
        [0x67, 0x86],  #flat high, flat low
        [0x68, 0xa2],  #[7:4]dd limit [1:0]dd ratio
        [0x69, 0x18],  #gain high th
        [0x6a, 0x0f],  #[7:4]dn_c slop          //[3]use post_gain [2]use pre_gain [1]use global gain [0]use col gain
        [0x6b, 0x00],  #[7:4]dn_b slop [3:0]dn_n slop
        [0x6c, 0x5f],  #[7:4]bright_th start [3:0]bright_th slop
        [0x6d, 0x8f],  #[7:4]dd_limit_start[3:0]dd_limit slop
        [0x6e, 0xa5],  #[7:4]ee1 effect start [3:0]slope  broad
        [0x6f, 0xa8],  #[7:4]ee2 effect start [3:0]slope  narrow
        [0x70, 0x15],  #saturation dec slope
        [0x71, 0x33],  #[7:4]low limit,[3:0]saturation slope
        [0x72, 0xdc],  #[7]edge_add_mode [6]new edge mode [5]edge2_mode [4]HP_mode
        [0x73, 0x80],  #[7]edge_add_mode2 [6]NA [5]only 2direction [4]fixed direction th
        [0x74, 0x02],  #direction th1
        [0x75, 0x3f],  #direction th2
        [0x76, 0x02],  #direction diff th      h>v+diff ; h>th1 ; v<th2
        [0x77, 0x11],  #[7:4]edge1_effect [3:0]edge2_effect  0x36
        [0x78, 0x88],  #[7:4]edge_pos_ratio  [3:0]edge neg ratio
        [0x79, 0x81],  #edge1_max,edge1_min
        [0x7a, 0x81],  #edge2_max,edge2_min
        [0x7b, 0x22],  #edge1_th,edge2_th
        [0x7c, 0xff],  #pos_edge_max,neg_edge_max
        [0x93, 0x48],  # <--40
        [0x94, 0x02],
        [0x95, 0x07],
        [0x96, 0xe0],
        [0x97, 0x40],
        [0x98, 0xf0],
        [0xb1, 0x40],  #manual cb
        [0xb2, 0x40],  #manual cr
        [0xb3, 0x30],  # x040
        [0xb6, 0xe0],
        [0xbd, 0x38],
        [0xbe, 0x36],  # [5:4]gray mode 00:4&8  01:4&12 10:4&20  11:8$16   [3:0] auto_gray
        [0xd0, 0xcb],  # exp is gc mode
        [0xd1, 0x10],  #every N
        [0xd2, 0x10],  # 7 aec enable 5 clore y mode 4skin weight 3 weight drop mode
        [0xd3, 0x48],  #Y_target and low pixel thd high X4 low X2
        [0xd5, 0xf2],  #lhig
        [0xd6, 0x16],  # ignore mode
        [0xdb, 0x92],
        [0xdc, 0xa5],  #fast_margin  fast_ratio
        [0xdf, 0x23],  # I_fram D_ratio
        [0xd9, 0x00],  # colore offset in CAL ,now is too dark so set zero
        [0xda, 0x00],  # GB offset
        [0xe0, 0x09],
        [0xed, 0x04],  #minimum exposure low 8  bits
        [0xee, 0xa0],  #max_post_dg_gain
        [0xef, 0x40],  #max_pre_dg_gain
        [0x80, 0x00],
        [0x80, 0x03],
        [0x9F, 0x10],
        [0xA0, 0x20],
        [0xA1, 0x38],
        [0xA2, 0x4E],
        [0xA3, 0x63],
        [0xA4, 0x76],
        [0xA5, 0x87],
        [0xA6, 0xA2],
        [0xA7, 0xB8],
        [0xA8, 0xCA],
        [0xA9, 0xD8],
        [0xAA, 0xE3],
        [0xAB, 0xEB],
        [0xAC, 0xF0],
        [0xAD, 0xF8],
        [0xAE, 0xFD],
        [0xAF, 0xFF],
        [0xc0, 0x00],  #Y_gamma_0
        [0xc1, 0x10],  #Y_gamma_1
        [0xc2, 0x1C],  #Y_gamma_2
        [0xc3, 0x30],  #Y_gamma_3
        [0xc4, 0x43],  #Y_gamma_4
        [0xc5, 0x54],  #Y_gamma_5
        [0xc6, 0x65],  #Y_gamma_6
        [0xc7, 0x75],  #Y_gamma_7
        [0xc8, 0x93],  #Y_gamma_8
        [0xc9, 0xB0],  #Y_gamma_9
        [0xca, 0xCB],  #Y_gamma_10
        [0xcb, 0xE6],  #Y_gamma_11
        [0xcc, 0xFF],  #Y_gamma_12
        [0xf0, 0x02],
        [0xf1, 0x01],
        [0xf2, 0x02],  #anual stretch K
        [0xf3, 0x30],  #he limit of Y_stretch
        [0xf7, 0x12],
        [0xf8, 0x0a],
        [0xf9, 0x9f],
        [0xfa, 0x78],
        [0xfe, 0x01],
        [0x00, 0xf5],  #high_low limit
        [0x02, 0x20],  #y2c
        [0x04, 0x10],
        [0x05, 0x08],
        [0x06, 0x20],
        [0x08, 0x0a],
        [0x0a, 0xa0],  # number limit
        [0x0b, 0x60],  # skip_mode
        [0x0c, 0x08],  #
        [0x0e, 0x44],  # width step
        [0x0f, 0x32],  # height step
        [0x10, 0x41],  #
        [0x11, 0x37],  # 0x3f
        [0x12, 0x22],  #
        [0x13, 0x19],  #13//smooth 2
        [0x14, 0x44],  #R_5k_gain_base
        [0x15, 0x44],  #B_5k_gain_base
        [0x16, 0xc2],  #c2//sinT
        [0x17, 0xa8],  #ac//a8//a8//a8//cosT
        [0x18, 0x18],  #X1 thd
        [0x19, 0x50],  #X2 thd
        [0x1a, 0xd8],  #e4//d0//Y1 thd
        [0x1b, 0xf5],  #Y2 thd
        [0x70, 0x40],  # A R2G low	
        [0x71, 0x58],  # A R2G high
        [0x72, 0x30],  # A B2G low
        [0x73, 0x48],  # A B2G high
        [0x74, 0x20],  # A G low
        [0x75, 0x60],  # A G high  
        [0x77, 0x20],
        [0x78, 0x32],
        [0x30, 0x03],  #[1]HSP_en [0]sa_curve_en
        [0x31, 0x40],
        [0x32, 0x10],
        [0x33, 0xe0],
        [0x34, 0xe0],
        [0x35, 0x00],
        [0x36, 0x80],
        [0x37, 0x00],
        [0x38, 0x04],  #sat1, at8   
        [0x39, 0x09],
        [0x3a, 0x12],
        [0x3b, 0x1C],
        [0x3c, 0x28],
        [0x3d, 0x31],
        [0x3e, 0x44],
        [0x3f, 0x57],
        [0x40, 0x6C],
        [0x41, 0x81],
        [0x42, 0x94],
        [0x43, 0xA7],
        [0x44, 0xB8],
        [0x45, 0xD6],
        [0x46, 0xEE],  #sat15,at224
        [0x47, 0x0d],  #blue_edge_dec_ratio  
        [0xfe, 0x00]]
	
	def __init__( self, parent ):

		if (os.path.exists(self.regfile)):
			f = open(self.regfile, 'r')
			data = f.read()
			self.reginfo = re.findall('0x[0-f][0-f]', data)
			for index in range(0, len(self.reginfo)):
				if index % 2:
					self.reg_values.append(self.reginfo[index])
				else:	
					self.regs.append(self.reginfo[index])
				
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = wx.EmptyString, pos = wx.DefaultPosition, size = wx.Size( 800,600 ), style = wx.DEFAULT_FRAME_STYLE|wx.HSCROLL|wx.TAB_TRAVERSAL )
		#self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetSizeHints( wx.DefaultSize, wx.DefaultSize )
		
		bSizer1 = wx.BoxSizer( wx.VERTICAL )
		
		bSizer5 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, u"Serial port:", wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
		self.m_staticText1.Wrap( -1 )
		bSizer5.Add( self.m_staticText1, 0, wx.EXPAND, 5 )
		
		if platform.system() == 'Linux':
			self.m_comboBox1Choices = ['ttyS0','ttyS1','ttyS2','ttyUSB0','ttyUSB1','ttyUSB2']
		else:
			self.m_comboBox1Choices = ['com1','com2','com3','com4','com5','com6']
		self.m_comboBox1 = wx.ComboBox( self, wx.ID_ANY, u"Slect com", wx.DefaultPosition, wx.DefaultSize, self.m_comboBox1Choices, 0 )
		self.m_comboBox1.SetSelection(0)
		bSizer5.Add( self.m_comboBox1, 0, wx.EXPAND, 5 )
		
		
		bSizer1.Add( bSizer5, 1, wx.EXPAND, 5 )
		
		bSizer2 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_staticText2 = wx.StaticText( self, wx.ID_ANY, u"File:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText2.Wrap( -1 )
		bSizer2.Add( self.m_staticText2, 0, wx.ALL, 5 )
		
		self.m_textCtrl1 = wx.TextCtrl( self, wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_CENTRE )
		bSizer2.Add( self.m_textCtrl1, 3, wx.ALL, 5 )
		
		self.m_button1 = wx.Button( self, wx.ID_ANY, u"...", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer2.Add( self.m_button1, 0, wx.ALL, 5 )
		
		
		bSizer1.Add( bSizer2, 1, wx.EXPAND, 5 )
		
		bSizer3 = wx.BoxSizer( wx.HORIZONTAL )
		
		#self.m_grid3 = wx.grid.Grid( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.HSCROLL ) #|wx.VSCROLL
		self.m_grid3 = wx.grid.Grid( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize ) #|wx.VSCROLL
		
		# Grid
		self.m_grid3.CreateGrid( 16, 16 )
		self.m_grid3.EnableEditing( True )
		self.m_grid3.EnableGridLines( True )
		self.m_grid3.EnableDragGridSize( False )
		self.m_grid3.SetMargins( 0, 0 )
		
		# Columns
		self.m_grid3.EnableDragColMove( False )
		self.m_grid3.EnableDragColSize( True )
		self.m_grid3.SetColLabelSize( 30 )
		self.m_grid3.SetColLabelValue( 0, u"0x0" )
		self.m_grid3.SetColLabelValue( 1, u"0x1" )
		self.m_grid3.SetColLabelValue( 2, u"0x02" )
		self.m_grid3.SetColLabelValue( 3, u"0x03" )
		self.m_grid3.SetColLabelValue( 4, u"0x4" )
		self.m_grid3.SetColLabelValue( 5, u"0x5" )
		self.m_grid3.SetColLabelValue( 6, u"0x6" )
		self.m_grid3.SetColLabelValue( 7, u"0x7" )
		self.m_grid3.SetColLabelValue( 8, u"0x8" )
		self.m_grid3.SetColLabelValue( 9, u"0x9" )
		self.m_grid3.SetColLabelValue( 10, u"0xa" )
		self.m_grid3.SetColLabelValue( 11, u"0xb" )
		self.m_grid3.SetColLabelValue( 12, u"0xc" )
		self.m_grid3.SetColLabelValue( 13, u"0xd" )
		self.m_grid3.SetColLabelValue( 14, u"0xe" )
		self.m_grid3.SetColLabelValue( 15, u"0xf" )
		self.m_grid3.SetColLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		
		# Rows
		self.m_grid3.EnableDragRowSize( True )
		self.m_grid3.SetRowLabelSize( 40 )
		self.m_grid3.SetRowLabelValue( 0, u"0x00" )
		self.m_grid3.SetRowLabelValue( 1, u"0x01" )
		self.m_grid3.SetRowLabelValue( 2, u"0x02" )
		self.m_grid3.SetRowLabelValue( 3, u"0x03" )
		self.m_grid3.SetRowLabelValue( 4, u"0x04" )
		self.m_grid3.SetRowLabelValue( 5, u"0x05" )
		self.m_grid3.SetRowLabelValue( 6, u"0x06" )
		self.m_grid3.SetRowLabelValue( 7, u"0x07" )
		self.m_grid3.SetRowLabelValue( 8, u"0x08" )
		self.m_grid3.SetRowLabelValue( 9, u"0x09" )
		self.m_grid3.SetRowLabelValue( 10, u"0x0a" )
		self.m_grid3.SetRowLabelValue( 11, u"0x0b" )
		self.m_grid3.SetRowLabelValue( 12, u"0x0c" )
		self.m_grid3.SetRowLabelValue( 13, u"0x0d" )
		self.m_grid3.SetRowLabelValue( 14, u"0x0e" )
		self.m_grid3.SetRowLabelValue( 15, u"0x0f" )
		self.m_grid3.SetRowLabelAlignment( wx.ALIGN_CENTRE, wx.ALIGN_CENTRE )
		

		self.UpdateTable()

		# Label Appearance
		
		# Cell Defaults
		self.m_grid3.SetDefaultCellAlignment( wx.ALIGN_LEFT, wx.ALIGN_TOP )
		bSizer3.Add( self.m_grid3, 0, wx.ALIGN_CENTER|wx.ALL|wx.EXPAND, 5 )
		
		
		bSizer1.Add( bSizer3, 10, wx.ALL, 5 )
		
		bSizer4 = wx.BoxSizer( wx.HORIZONTAL )
		
		self.m_button2 = wx.Button( self, wx.ID_ANY, u"start", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.m_button2, 0, wx.ALL, 5 )
		
		self.m_button3 = wx.Button( self, wx.ID_ANY, u"stop", wx.DefaultPosition, wx.DefaultSize, 0 )
		bSizer4.Add( self.m_button3, 0, wx.ALL, 5 )
		
		
		bSizer1.Add( bSizer4, 1, wx.EXPAND, 5 )
		
		
		self.SetSizer( bSizer1 )
		self.Layout()
		
		self.Centre( wx.BOTH )
		
		self.Bind(wx.EVT_BUTTON, self.OnSlectFile, self.m_button1)
		self.Bind(wx.EVT_BUTTON, self.OnStart, self.m_button2)
		self.Bind(wx.EVT_BUTTON, self.OnStop, self.m_button3)
		#self.Bind(wx.grid.EVT_GRID_SELECT_CELL, self.OnSelectCell, self.m_grid3)
		self.Bind(wx.grid.EVT_GRID_CELL_CHANGED, self.OnChangeCellValue, self.m_grid3)
	
	def __del__( self ):
		pass

	def savefile(self,data):
		filename = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		filename = self.filename + '/' + filename
		data_bin = numpy.array(data)
		fd = open(filename+'.yuv', 'wb')
		for val in data:
			a = struct.pack('B', val)
			fd.write(a)
		fd.close() 

	def UpdateTable(self):
		#test set values
		for row in range(0,16):
			for col in range(0,16):
				self.m_grid3.SetCellValue( col, row, u"0x%02x"% int( self.reg_values[row*16+col][2:], 16 ))
		pass

	def OnSelectCell(self, event):
		pass

	def OnChangeCellValue(self, event):
		row = event.GetRow()
		col = event.GetCol()
		cellvalue = self.m_grid3.GetCellValue(event.GetRow(), event.GetCol())
		self.reg_values[col*16 + row] = cellvalue
		self.UpdateTable()
		fd = open(self.regfile, 'w')
		datas = ""
		for i in range(0,256):
			datas = datas + self.regs[i] + "," + self.reg_values[i] + "\n"
		fd.write(datas)
		fd.close()
			
		self.runreg()
		pass

	def OnStart(self, event):
		self.serialdev = self.m_comboBox1Choices[self.m_comboBox1.GetSelection()]
		self.serialdev = "/dev/" + self.serialdev
		if self.cmd == None:
    			self.cmd = uartcmd.UartCmd(self.serialdev, 1500000)
		print(self.serialdev)
		if (not self.filename):
			self.Warning( "Please Select file!!!!")
			return
			#sys.exit(1)

		if (not os.path.exists(self.regfile)):
			self.Warning("Please prepare finger.reg!!!!")
			return
			#sys.exit(1)
		self.stop_flag = 0
		self.runreg()
	def run_thread(self):
		while True:
			if self.stop_flag:
				break
			data = self.cmd.recv_data()
			if data:
				self.savefile(data)
		
	def runreg(self):
		#cmd = "uart-finger -d " + serialdev + " -f " + self.regfile + " -p " + self.filename
	#self.filename = /home
		self.cmd.send_regs(flatten(self.reg))
		#t = threading.Thread(target=self.run_thread,args=())
		#t.setDaemon(True)
		#t.start()
		
		data = self.cmd.recv_data(0)
		if data:
			self.savefile(data)
		#cmd = "./uart-finger -d " + self.serialdev  + " -p " + self.filename
		#if self.popen != None:
		#	self.popen.kill()
		#	
		#self.popen = subprocess.Popen(cmd, shell=True)
		#os.system(cmd)
	
	def OnStop(self, event):
		self.stop_flag = 1
		pass
		#if self.popen != None:
		#	self.popen.kill()
            
	def OnSlectFile(self, event):
		#file_wildcard = "Paint files(*.yuv)|*.paint|All files(*.*)|*.*" 
		#dlg = wx.FileDialog(self, 
		#                    "Save paint as ...",
		#                    os.getcwd(),
		#                    #style = wx.SAVE | wx.OVERWRITE_PROMPT,
		#                    style = wx.FD_OPEN,
		#                    #style = wx.FD_CHANGE_DIR,
		#                    wildcard = file_wildcard)
		#if dlg.ShowModal() == wx.ID_OK:
		#    filename = dlg.GetPath()
		#    if not os.path.splitext(filename)[1]:
		#        filename = filename + '.yuv'
		#    self.filename = os.path.dirname(filename)
		#    #self.SaveFile()
		#    #self.SetTitle(self.title + '--' + self.filename)
		#dlg.Destroy() 
		#self.m_textCtrl1.SetValue(self.filename)
		dialog = wx.DirDialog(None, "Choose a directory:",
		      style=wx.DD_DEFAULT_STYLE | wx.DD_NEW_DIR_BUTTON)
		if dialog.ShowModal() == wx.ID_OK:
		    path = dialog.GetPath()
		    self.filename = path
		dialog.Destroy()
	
		pass

	def Warning(self, msg):
		ww = warning_window(self,msg)
		ww.ShowModal()
		ww.Destroy()



if __name__ == '__main__':
    #app = wx.PySimpleApp()
    app = wx.App()
    frame = MyFrame1(None)
    frame.Show(True)
    app.MainLoop()
