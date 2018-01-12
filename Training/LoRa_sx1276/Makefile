include common.mk

test_board: $(ELF)
# Otherwise, custom_board.h is selected instead of test_board.h
test_board: CFLAGS += -D TEST_BOARD
