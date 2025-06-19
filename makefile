DIR := Q_1 Q_2 Q_3 Q_4 Q_5 Q_6 Q_7 Q_8 Q_9 Q_10

.PHONY: all

all:
	@for dir in $(DIR); do \
		echo "===> Entering $$dir"; \
		$(MAKE) -C $$dir || echo "Build failed in $$dir"; \
	done


.PHONY: all clean

DIR := Q_1 Q_2 Q_3 Q_4 Q_5 Q_6 Q_7 Q_8 Q_9 Q_10

all:
	@for dir in $(DIR); do \
		echo "===> Building $$dir"; \
		$(MAKE) -C $$dir || echo "Build failed in $$dir"; \
	done

clean:
	@for dir in $(DIR); do \
		echo "===> Cleaning $$dir"; \
		$(MAKE) -C $$dir clean || echo "Clean failed in $$dir"; \
	done
