<CsoundSynthesizer>
<CsOptions>
-n -iadc -b128 -B256
</CsOptions>
<CsInstruments>

sr  = 44100
ksmps = 500
nchnls = 2
0dbfs  = 1

instr 1
	
	iPort serialBegin "/dev/cu.usbmodem1411", 115200
	
	aSig in
	kRMS rms aSig
	kRMS = kRMS * 255 * 30
	;kRMS = 255 - kRMS
	kRMS portk kRMS, .5
	serialWrite iPort, (kRMS < 255 ? kRMS : 255)

endin

</CsInstruments>
<CsScore>
i 1 0 999999
e
</CsScore>
</CsoundSynthesizer>