test: test-convert test-gp test-gp-rd test-gen-palette

test-convert:
	for i in *.png; do \
		for j in RGB RGBA GRAY GA; do \
			for k in 8 16; do \
				out=$$i.$$j.$$k.out; \
				../test-convert $$j $$k $$i $$out \
				&& cmp $$out cmp/$$out; \
			done; \
		done; \
	done

test-gp:
	for i in 1 2 4; do \
		out=gp$$i.png.out; \
		../test-gp $$i $$out && cmp $$out cmp/$$out; \
	done

test-gp-rd:
	for i in 1 2 4; do \
		in=basn0g0$$i.png; \
		out=$$in.out; \
		../test-gp-rd $$i $$in $$out && cmp $$out cmp/$$out; \
	done

test-gen-palette:
	../test-gen-palette
	for i in 1 2 4 8; do \
		out=palette$$i.png.out; \
		cmp $$out cmp/$$out; \
	done

clean:
	rm -f *.out

.PHONY: test clean
