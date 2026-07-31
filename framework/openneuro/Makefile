all: cleanup
	$(MAKE) -C ./errors
	$(MAKE) -C ./api
	$(MAKE) -C ./math
	$(MAKE) -C ./models
	$(MAKE) extract

cleanup:
	rm -f ./bin/*
	rm -rf ./include/*

extract:
	mkdir -p ./bin/
	mkdir -p ./include/openneu
	find . -path "./bin" -prune -o -name "*.a" -exec cp {} bin/ \;
	find . -path "./include" -prune -o -name "*.h" -exec cp {} include/openneu/ \;
