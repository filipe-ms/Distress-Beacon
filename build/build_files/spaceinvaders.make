# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug_x64
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
INCLUDES += -I../../src -I../../include -I../external/raylib-master/src -I../external/raylib-master/src/external -I../external/raylib-master/src/external/glfw/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug_x64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/x64/Debug/spaceinvaders
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64

else ifeq ($(config),debug_x86)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/x86/Debug/spaceinvaders
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32

else ifeq ($(config),debug_arm64)
TARGETDIR = ../../bin/Debug
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/ARM64/Debug/spaceinvaders
DEFINES += -DDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -g -std=c++17
LIBS += ../../bin/Debug/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Debug/libraylib.a
ALL_LDFLAGS += $(LDFLAGS)

else ifeq ($(config),release_x64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/x64/Release/spaceinvaders
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m64 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib64 -m64 -s

else ifeq ($(config),release_x86)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/x86/Release/spaceinvaders
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -m32 -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -L/usr/lib32 -m32 -s

else ifeq ($(config),release_arm64)
TARGETDIR = ../../bin/Release
TARGET = $(TARGETDIR)/spaceinvaders
OBJDIR = obj/ARM64/Release/spaceinvaders
DEFINES += -DNDEBUG -DPLATFORM_DESKTOP -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11 -D_GNU_SOURCE
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c99
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -Wshadow -O2 -std=c++17
LIBS += ../../bin/Release/libraylib.a -lpthread -lm -ldl -lrt -lX11
LDDEPS += ../../bin/Release/libraylib.a
ALL_LDFLAGS += $(LDFLAGS) -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/audio_manager.o
GENERATED += $(OBJDIR)/background.o
GENERATED += $(OBJDIR)/bars.o
GENERATED += $(OBJDIR)/dev_interface_left_ui.o
GENERATED += $(OBJDIR)/draw_utils.o
GENERATED += $(OBJDIR)/enemy.o
GENERATED += $(OBJDIR)/enemy_projectile.o
GENERATED += $(OBJDIR)/enemy_wave.o
GENERATED += $(OBJDIR)/enter_name.o
GENERATED += $(OBJDIR)/game.o
GENERATED += $(OBJDIR)/game_behavior.o
GENERATED += $(OBJDIR)/game_over.o
GENERATED += $(OBJDIR)/general_utils.o
GENERATED += $(OBJDIR)/input.o
GENERATED += $(OBJDIR)/left_ui.o
GENERATED += $(OBJDIR)/list.o
GENERATED += $(OBJDIR)/main.o
GENERATED += $(OBJDIR)/main_menu.o
GENERATED += $(OBJDIR)/pilot.o
GENERATED += $(OBJDIR)/player.o
GENERATED += $(OBJDIR)/power_ups.o
GENERATED += $(OBJDIR)/ranking.o
GENERATED += $(OBJDIR)/scene_draw_effects.o
GENERATED += $(OBJDIR)/scene_manager.o
GENERATED += $(OBJDIR)/select_ship.o
GENERATED += $(OBJDIR)/ship.o
GENERATED += $(OBJDIR)/ship_references.o
GENERATED += $(OBJDIR)/special_effects.o
GENERATED += $(OBJDIR)/texture_manager.o
GENERATED += $(OBJDIR)/timer.o
GENERATED += $(OBJDIR)/tutorial.o
GENERATED += $(OBJDIR)/user_interface.o
GENERATED += $(OBJDIR)/weapon.o
GENERATED += $(OBJDIR)/winner.o
OBJECTS += $(OBJDIR)/audio_manager.o
OBJECTS += $(OBJDIR)/background.o
OBJECTS += $(OBJDIR)/bars.o
OBJECTS += $(OBJDIR)/dev_interface_left_ui.o
OBJECTS += $(OBJDIR)/draw_utils.o
OBJECTS += $(OBJDIR)/enemy.o
OBJECTS += $(OBJDIR)/enemy_projectile.o
OBJECTS += $(OBJDIR)/enemy_wave.o
OBJECTS += $(OBJDIR)/enter_name.o
OBJECTS += $(OBJDIR)/game.o
OBJECTS += $(OBJDIR)/game_behavior.o
OBJECTS += $(OBJDIR)/game_over.o
OBJECTS += $(OBJDIR)/general_utils.o
OBJECTS += $(OBJDIR)/input.o
OBJECTS += $(OBJDIR)/left_ui.o
OBJECTS += $(OBJDIR)/list.o
OBJECTS += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/main_menu.o
OBJECTS += $(OBJDIR)/pilot.o
OBJECTS += $(OBJDIR)/player.o
OBJECTS += $(OBJDIR)/power_ups.o
OBJECTS += $(OBJDIR)/ranking.o
OBJECTS += $(OBJDIR)/scene_draw_effects.o
OBJECTS += $(OBJDIR)/scene_manager.o
OBJECTS += $(OBJDIR)/select_ship.o
OBJECTS += $(OBJDIR)/ship.o
OBJECTS += $(OBJDIR)/ship_references.o
OBJECTS += $(OBJDIR)/special_effects.o
OBJECTS += $(OBJDIR)/texture_manager.o
OBJECTS += $(OBJDIR)/timer.o
OBJECTS += $(OBJDIR)/tutorial.o
OBJECTS += $(OBJDIR)/user_interface.o
OBJECTS += $(OBJDIR)/weapon.o
OBJECTS += $(OBJDIR)/winner.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking spaceinvaders
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning spaceinvaders
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/audio_manager.o: ../../src/audio_manager.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/background.o: ../../src/background.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/bars.o: ../../src/bars.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/dev_interface_left_ui.o: ../../src/dev_interface_left_ui.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/draw_utils.o: ../../src/draw_utils.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/enemy.o: ../../src/enemy.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/enemy_projectile.o: ../../src/enemy_projectile.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/enemy_wave.o: ../../src/enemy_wave.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/enter_name.o: ../../src/enter_name.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/game.o: ../../src/game.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/game_behavior.o: ../../src/game_behavior.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/game_over.o: ../../src/game_over.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/general_utils.o: ../../src/general_utils.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/input.o: ../../src/input.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/left_ui.o: ../../src/left_ui.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/list.o: ../../src/list.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: ../../src/main.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main_menu.o: ../../src/main_menu.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/pilot.o: ../../src/pilot.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/player.o: ../../src/player.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/power_ups.o: ../../src/power_ups.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ranking.o: ../../src/ranking.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/scene_draw_effects.o: ../../src/scene_draw_effects.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/scene_manager.o: ../../src/scene_manager.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/select_ship.o: ../../src/select_ship.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ship.o: ../../src/ship.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/ship_references.o: ../../src/ship_references.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/special_effects.o: ../../src/special_effects.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/texture_manager.o: ../../src/texture_manager.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/timer.o: ../../src/timer.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/tutorial.o: ../../src/tutorial.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/user_interface.o: ../../src/user_interface.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/weapon.o: ../../src/weapon.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/winner.o: ../../src/winner.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif