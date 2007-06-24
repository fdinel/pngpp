test: test-convert test-gp

test-convert:
	for i in *.png; do \
		for j in RGB RGBA GRAY GA; do \
			out=$$i.$$j.out; \
			../test-convert $$j $$i $$out && cmp $$out cmp/$$out; \
		done; \
	done

test-gp:
	for i in 1 2 4; do \
		out=gp$$i.png.out; \
		../test-gp $$i $$out && cmp $$out cmp/$$out; \
	done

clean:
	rm -f *.out

.PHONY: test clean
