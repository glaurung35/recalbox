import typing

from configgen.Emulator import Emulator
from configgen.crtswitchres.CRTTypes import CRTResolutionType, CRTScreenType, CRTScanlines, RetroarchScreenTypeIndex
from configgen.utils.Rotation import Rotation
from configgen.utils.architecture import Architecture


class LibretroConfigCRTSwitchres:

    def manage_scanlines(self, system: Emulator, config: typing.Dict[str, str]):
        if system.CRTScanlines != CRTScanlines.NONE:
            if (system.CRTScreenType == CRTScreenType.kHz31 and (system.CRTResolutionType == CRTResolutionType.Force480p or system.CRTResolutionType == CRTResolutionType.Auto)) \
                    or (system.CRTScreenType == CRTScreenType.kHzTriFreq or system.CRTScreenType == CRTScreenType.kHzMulti1531) and system.CRTResolutionType == CRTResolutionType.Force480p:
                if (system.Rotation.isTate() and system.VerticalGame) or (not system.Rotation.isTate() and not system.VerticalGame):
                    config.update({"video_shader_enable": '"true"'})
                    config.update({"video_shader_dir": '"/recalbox/share/shaders/"'})
                    config.update({"video_shader": '/recalbox/share/shaders/rrgbd-scanlines-{}.glslp'.format(system.CRTScanlines)})

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

        match system.CRTScreenType:
            case CRTScreenType.kHz15:
                config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz15.value
            case CRTScreenType.kHz31:
                if system.CRTResolutionType == CRTResolutionType.DoubleFreq:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz31at120.value
                else:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz31.value
            case CRTScreenType.kHzMulti1531:
                if system.CRTResolutionType == CRTResolutionType.Force480p:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz31.value
                else:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHzMulti1531.value
            case CRTScreenType.kHzMulti1525:
                if system.CRTResolutionType == CRTResolutionType.Force240p:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz15.value
                else:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHzMulti1525.value
            case CRTScreenType.kHzTriFreq:
                if system.CRTResolutionType == CRTResolutionType.Force240p:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz15.value
                elif system.CRTResolutionType == CRTResolutionType.Force480p:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHz31.value
                else:
                    config["crt_switch_resolution"] = RetroarchScreenTypeIndex.kHzTriFreq.value

        config["crt_switch_resolution_super"] = system.CRTSuperrez
        pi5 = Architecture.isPi5
        if pi5 or system.CRTResolutionType != CRTResolutionType.Auto:
            config["crt_switch_resolution_no_interlaced"] = 1
        else:
            config["crt_switch_resolution_no_interlaced"] = 0

        if system.Rotation.isTate():
            config["video_rotation"] = system.Rotation

        self.manage_scanlines(system, config)

        return config
