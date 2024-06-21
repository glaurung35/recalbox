import typing

from configgen.Emulator import Emulator
from configgen.crt.CRTConfigParser import CRTConfigParser, CRTArcadeMode, CRTVideoStandard, CRTScreenType, \
    CRTResolutionType, CRTSystem
from configgen.utils.Rotation import Rotation
from configgen.utils.architecture import Architecture


class LibretroConfigCRTSwitchres:

    #def manage_scanlines(self, system: Emulator, config:typing.Dict[str, str], vertical: bool):
        # if system.CRTScreenType == CRTScreenType.k31 and system.CRTScanlines != CRTScanlines.NONE and system.CRTResolutionType == CRTResolutionType.Progressive:
        #     if (system.Rotation.isTate() and vertical) or (not system.Rotation.isTate() and not vertical):
        #         config.update({"video_shader_enable": '"true"'})
        #         config.update({"video_shader_dir": '"/recalbox/share/shaders/"'})
        #         config.update({"video_shader": '/recalbox/share/shaders/rrgbd-scanlines-{}.glslp'.format(system.CRTScanlines)})

    def createConfigFor(self, system: Emulator, rom: str) -> typing.Dict[str, str]:
        config: typing.Dict[str, any] = {"aspect_ratio_index": "22",
                                         "video_aspect_ratio_auto": 'true',
                                         "crt_switch_timings_pal": "",
                                         "crt_switch_timings_ntsc": "",
                                         "video_refresh_rate_pal": '',
                                         "video_refresh_rate_ntsc": '',
                                         "crt_switch_timings": "",
                                         "custom_viewport_width": "",
                                         "custom_viewport_width_ntsc": "",
                                         "custom_viewport_width_pal": "",
                                         "custom_viewport_height": "",
                                         "custom_viewport_height_ntsc": "",
                                         "custom_viewport_height_pal": "",
                                         "custom_viewport_x": "",
                                         "custom_viewport_x_ntsc": "",
                                         "custom_viewport_x_pal": "",
                                         "custom_viewport_y": "",
                                         "custom_viewport_y_ntsc": "",
                                         "custom_viewport_y_pal": "",
                                         "video_crop_overscan": '',
                                         "video_fullscreen": '',
                                         "video_fullscreen_x": '',
                                         "video_fullscreen_y": '',
                                         "menu_driver": '"rgui"',
                                         "video_shader_enable": '"false"',
                                         "video_vsync": '"true"',
                                         "video_black_frame_insertion": '"0"',
                                         "audio_driver": '"pulse"',
                                         "video_smooth": '"false"',
                                         "video_allow_rotate": '"true"',
                                         "video_rotation": 0,
                                         "video_driver": '"gl"',
                                         "video_scale_integer_overscale": ''
                                         }

        config["crt_switch_resolution"] = 1
        config["crt_switch_resolution_super"] = system.CRTSuperrez
        pi5 = Architecture().isPi5
        if pi5:
            config["crt_switch_resolution_no_interlaced"] = 1
        else:
            config["crt_switch_resolution_no_interlaced"] = 0

        if system.Rotation.isTate():
                config["video_rotation"] = system.Rotation

        return config
