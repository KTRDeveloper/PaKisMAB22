
all:
	##################################################
	###               kissat		       ###
	##################################################
#	( cd ./kissat && ./configure --competition)
	( cd ./kissat && ./configure --quiet)
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
	###               PaKiss		       ###
	##################################################
	+ $(MAKE) -C kissat clean

	##################################################
	###                 PaKiss                     ###
	##################################################
	+ $(MAKE) clean -C painless-src
	rm -f PaKis
