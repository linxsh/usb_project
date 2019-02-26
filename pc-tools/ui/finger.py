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
#import serial

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
		print(self.serialdev)
		if (not self.filename):
			self.Warning( "Please Select file!!!!")
			return
			#sys.exit(1)

		if (not os.path.exists(self.regfile)):
			self.Warning("Please prepare finger.reg!!!!")
			return
			#sys.exit(1)
		self.runreg()
		
	def runreg(self):
		#cmd = "uart-finger -d " + serialdev + " -f " + self.regfile + " -p " + self.filename
	#self.filename = /home
		cmd = "./uart-finger -d " + self.serialdev  + " -p " + self.filename
		if self.popen != None:
			self.popen.kill()
			
		self.popen = subprocess.Popen(cmd, shell=True)
		#os.system(cmd)
	
	def OnStop(self, event):
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
