test:
	for i in *.png; do \
		for j in RGB RGBA GRAY GA; do \
			out=$$i.$$j.out; \
			$(PNGPPTEST) $$j $$i $$out && cmp $$out cmp/$$out; \
		done; \
	done

clean:
	rm -f *.out

.PHONY: test clean
