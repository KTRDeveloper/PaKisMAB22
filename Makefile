
all:
	##################################################
	###               kissat		       ###
	##################################################
#	( cd ./kissat && ./configure --competition)
	( cd ./kissat && ./configure --quiet --compact --no-proofs --no-statistics)
	+ $(MAKE) -C kissat

	##################################################
	###                 PaKis                      ###
	##################################################
	+ $(MAKE) -C painless-src
	mv painless-src/painless PaKis
para:
	+ $(MAKE) -C painless-src
	mv painless-src/painless PaKis
clean:
	##################################################
	###               PaKis 		       ###
	##################################################
	+ $(MAKE) -C kissat clean

	##################################################
	###                 PaKis                      ###
	##################################################
	+ $(MAKE) clean -C painless-src
	rm -f PaKis
