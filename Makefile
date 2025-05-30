V := 1

include $(DEVKITPRO)/wut/share/wut_rules


TARGET := imgui-test-wiiu.elf

RPX_TARGET := $(TARGET:.elf=.rpx)
WUHB_TARGET := $(RPX_TARGET:.rpx=.wuhb)

SOURCES := \
	src/main.cpp \
	src/sdl_debug.cpp \
	src/stdout.cpp \
	external/imgui/imgui.cpp \
	external/imgui/imgui_demo.cpp \
	external/imgui/imgui_draw.cpp \
	external/imgui/imgui_tables.cpp \
	external/imgui/imgui_widgets.cpp \
	external/imgui/backends/imgui_impl_sdl2.cpp \
	external/imgui/backends/imgui_impl_sdlrenderer2.cpp


PKGCONFIG := powerpc-eabi-pkg-config

CPPFLAGS := \
	-DESPRESSO -D__WIIU__ -D__WUT__ \
	-I$(WUT_ROOT)/include \
	$(shell $(PKGCONFIG) --cflags sdl2) \
	-Iexternal/imgui \
	-DIMGUI_DISABLE_DEFAULT_SHELL_FUNCTIONS \
	-DIMGUI_DEFINE_MATH_OPERATORS=1 \
	-DIMGUI_RGBA_BE


CXX ?= g++
CXX += -std=c++23

CXXFLAGS := \
	-Wall -Wextra -Werror \
	-Og -ggdb -g3 \
	-mcpu=750 -meabi -mhard-float

LDFLAGS := \
	-L$(WUT_ROOT)/lib \
	$(shell $(PKGCONFIG) --libs sdl2) \
	$(RPXSPECS)

OBJECTS := $(SOURCES:.cpp=.o)


.PHONY: all clean company run


all: $(WUHB_TARGET)


clean:
	$(RM) $(TARGET)
	$(RM) $(RPX_TARGET)
	$(RM) $(WUHB_TARGET)
	$(RM) $(OBJECTS)



$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


$(WUHB_TARGET): $(RPX_TARGET)
	wuhbtool $< $@ \
		--content=assets \
		--name="ImGui Test" \
		--short-name="imgui-test"


%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@



company: compile_flags.txt

compile_flags.txt: $(firstword $(MAKEFILE_LIST))
	printf "%s" "$(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)" | xargs -n1 | sort -u > compile_flags.txt
	$(CPP) -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,-I,p' >> compile_flags.txt


run: $(WUHB_TARGET)
	WIILOAD=tcp:wiiu wiiload $<

