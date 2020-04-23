# export LIBRARY_PATH=$(CURDIR)/cosy/lib/:$(CURDIR)/cosy/third_party/automorphism/bliss/:
# export CPATH=$(CURDIR)/cosy/include/:$(CURDIR)/cosy/third_party/automorphism/:

all:
	##################################################
	###                  Cosy                      ###
	##################################################
	# + $(MAKE) -C cosy third_party
	# + $(MAKE) -C cosy
	##################################################
	###               MapleLCMDistChronoBT                  ###
	##################################################
	# cd mapleCOMSPS && tar zxvf m4ri-20140914.tar.gz
	# cd mapleCOMSPS/m4ri-20140914 && ./configure
	# + $(MAKE) -C mapleCOMSPS/m4ri-20140914
	+ $(MAKE) -C MapleLCMDistChronoBT

	##################################################
	###                 PaInleSS                   ###
	##################################################
	+ $(MAKE) -C painless-src
	mv painless-src/painless painless-MapleLCMDistChronoBT

clean:
	##################################################
	###                  Cosy                      ###
	##################################################
	# + $(MAKE) -C cosy clean-third_party
	# + $(MAKE) -C cosy cleanall

	##################################################
	###               MapleLCMDistChronoBT                  ###
	##################################################
	#rm -rf mapleCOMSPS/m4ri-20140914
	+ $(MAKE) -C MapleLCMDistChronoBT clean

	##################################################
	###                 PaInleSS                   ###
	##################################################
	+ $(MAKE) clean -C painless-src
	rm -f painless-MapleLCMDistChronoBT
