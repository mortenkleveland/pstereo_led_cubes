<CsoundSynthesizer>
<CsOptions>
-n -iadc 
</CsOptions>
<CsInstruments>

sr  = 44100
ksmps = 500
nchnls = 2
0dbfs  = 1

instr 1
	
	iPort serialBegin "/dev/cu.usbmodemfa131", 9600
	
	aSig in
	kRms rms aSig * kGain

	kRms = kRms * kRms * 255
	serialWrite iPort, (kRms < 255 ? kRms : 255)

endin
</CsInstruments>
<CsScore>
f 1 0 1024 10 1 1 1 1 1 1

i 1 0 200
e
</CsScore>
</CsoundSynthesizer>