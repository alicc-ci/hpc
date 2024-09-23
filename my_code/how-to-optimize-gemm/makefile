# OLD  := MMult0
# NEW  := blas_cal


# CC         := gcc
# LINKER     := $(CC) 

# CFLAGS     := -Wall -Werror -Wno-unused-result -Wno-unused-value -Wno-unused-variable -g -I/usr/include/x86_64-linux-gnu
# LDFLAGS    := -L/usr/lib/x86_64-linux-gnu -lopenblas -lm

# DATA_DIR = _data
# BUILD_DIR = _build
# OBJS  := $(BUILD_DIR)/util.o $(BUILD_DIR)/REF_MMult.o $(BUILD_DIR)/test_MMult.o $(BUILD_DIR)/$(NEW).o

# $(shell mkdir -p $(BUILD_DIR) $(DATA_DIR))

# $(BUILD_DIR)/%.o: %.c
# 	@mkdir -p $(dir $@) && echo + CC $<
# 	$(CC) -std=gnu11 $(CFLAGS) -c $< -o $@ 


# all:
# 	make clean;
# 	make $(BUILD_DIR)/test_MMult.x

# $(BUILD_DIR)/test_MMult.x: $(OBJS) defs.h
# 	$(LINKER) $(OBJS) $(LDFLAGS) -o $@

# export OMP_NUM_THREADS=1
# export GOTO_NUM_THREADS=1

# run:
# 	make all
# 	echo $$OMP_NUM_THREADS
# 	@echo "date = '`date`';" > $(DATA_DIR)/output_$(NEW).m
# 	@echo "version = '$(NEW)';" >> $(DATA_DIR)/output_$(NEW).m
# 	$(BUILD_DIR)/test_MMult.x >> $(DATA_DIR)/output_$(NEW).m
# 	@if [ ! -f $(DATA_DIR)/output_old.m ] || [ $(OLD) != $(NEW) ]; then \
#         cp $(DATA_DIR)/output_$(OLD).m $(DATA_DIR)/output_old.m; \
#     fi

# 	cp $(DATA_DIR)/output_$(NEW).m $(DATA_DIR)/output_new.m

# clean:
# 	rm -rf _build *.jpg

# cleanall:
# 	rm -rf _build _data *.jpg


OLD  := MMult0
NEW  := MY_MMult

CC         := gcc
LINKER     := $(CC) 

CFLAGS     := -Wall -Werror -Wno-unused-result -Wno-unused-value -Wno-unused-variable -g -I/usr/include/x86_64-linux-gnu -fopenmp
LDFLAGS    := -L/usr/lib/x86_64-linux-gnu -lopenblas -lm -fopenmp


DATA_DIR = _data
BUILD_DIR = _build
OBJS  := $(BUILD_DIR)/util.o $(BUILD_DIR)/REF_MMult.o $(BUILD_DIR)/test_MMult.o $(BUILD_DIR)/blas_cal.o $(BUILD_DIR)/MY_MMult.o $(BUILD_DIR)/openmp_gemm.o $(BUILD_DIR)/pthread_gemm.o


$(shell mkdir -p $(BUILD_DIR) $(DATA_DIR))

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@) && echo + CC $<
	$(CC) -std=gnu11 $(CFLAGS) -c $< -o $@ 

# $(BUILD_DIR)/MY_MMult.o: MY_MMult.c
#     $(CC) $(CFLAGS) -c MY_MMult.c -o 
# $(BUILD_DIR)/MY_MMult.o

# $(BUILD_DIR)/openmp_gemm.o: openmp_gemm.c
#     $(CC) $(CFLAGS) -c openmp_gemm.c -o 
# $(BUILD_DIR)/openmp_gemm.o


all: 
	make $(BUILD_DIR)/test_MMult.x

$(BUILD_DIR)/test_MMult.x: $(OBJS) defs.h
	$(LINKER) $(OBJS) $(LDFLAGS) -o $@



export OMP_NUM_THREADS=1
export GOTO_NUM_THREADS=1

run:
	make all
	echo $$OMP_NUM_THREADS
	@echo "date = '`date`';" > $(DATA_DIR)/output_$(NEW).m
	@echo "version = '$(NEW)';" >> $(DATA_DIR)/output_$(NEW).m
	$(BUILD_DIR)/test_MMult.x >> $(DATA_DIR)/output_$(NEW).m
	@if [ ! -f $(DATA_DIR)/output_old.m ] || [ $(OLD) != $(NEW) ]; then \
        -cp $(DATA_DIR)/output_$(OLD).m $(DATA_DIR)/output_old.m; \
    fi

	cp $(DATA_DIR)/output_$(NEW).m $(DATA_DIR)/output_new.m

clean:
	rm -rf _build *.jpg

cleanall:
	rm -rf _build _data *.jpg
