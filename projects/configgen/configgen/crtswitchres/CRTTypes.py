import typing
from dataclasses import dataclass
from enum import Enum, StrEnum


class CRTSuperRez(StrEnum):
    dynamic = "1"
    native = "0"
    x6 = "1920"
    x8 = "2560"

    @staticmethod
    def fromString(value: str) -> typing.Optional["CRTSuperRez"]:
        if value == "1":
            return CRTSuperRez.dynamic
        if value == "0":
            return CRTSuperRez.native
        if value == "1920":
            return CRTSuperRez.x6
        if value == "2560":
            return CRTSuperRez.x8
        return CRTSuperRez.x6


class CRTVideoStandard(StrEnum):
    PAL = "pal"
    NTSC = "ntsc"
    AUTO = "auto"
    ALL = "all"

    @staticmethod
    def fromString(value: str):
        if value == "pal":
            return CRTVideoStandard.PAL
        if value == "ntsc":
            return CRTVideoStandard.NTSC
        return CRTVideoStandard.AUTO

class CRTAdapter(StrEnum):
    RECALBOXRGBDUAL = "recalboxrgbdual"
    RECALBOXRGBJAMMA = "recalboxrgbjamma"
    OTHER = "other"
    NONE = "none"

    @staticmethod
    def fromString(value: str):
        if value == "recalboxrgbdual":
            return CRTAdapter.RECALBOXRGBDUAL
        if value == "recalboxrgbjamma":
            return CRTAdapter.RECALBOXRGBJAMMA
        if len(value) > 0:
            return CRTAdapter.OTHER
        return CRTAdapter.NONE

class CRTScanlines(StrEnum):
    LIGHT = "light"
    MEDIUM = "medium"
    HEAVY = "heavy"
    NONE = "none"

    @staticmethod
    def fromString(value: str):
        if value == "light":
            return CRTScanlines.LIGHT
        if value == "medium":
            return CRTScanlines.MEDIUM
        if value == "heavy":
            return CRTScanlines.HEAVY
        return CRTScanlines.NONE


class CRTRegion(StrEnum):
    AUTO = "auto"
    EU = "eu"
    US = "us"
    JP = "jp"

    @staticmethod
    def fromString(value: str):
        if value == "eu":
            return CRTRegion.EU
        if value == "us":
            return CRTRegion.US
        if value == "jp":
            return CRTRegion.JP
        return CRTRegion.AUTO


class CRTScreenType(StrEnum):
    auto = "Auto"
    kHz15 = "kHz15"
    kHzMulti1525 = "kHzMulti1525"
    kHz31 = "kHz31"
    kHzMulti1531 = "kHzMulti1531"
    kHzTriFreq = "kHzTriFreq"

    @classmethod
    def fromString(cls, value: str):
        for k, v in cls.__members__.items():
            if v == value:
                return v
        return CRTScreenType.kHz15

class RetroarchScreenTypeIndex(Enum):
    disabled = 0
    kHz15 = 1
    kHzMulti1525 = 2
    kHz31 = 3
    kHzMulti1531 = 4
    kHzTriFreq = 5
    kHz31at120 = 6
    kHz15Jamma = 7


class CRTResolutionType(StrEnum):
    Auto = "auto"
    Force240p = "240p"
    Force480p = "480p"
    DoubleFreq = "doublefreq"

    @classmethod
    def fromString(cls, value: str):
        for k, v in cls.__members__.items():
            if v == value:
                return v
        return CRTResolutionType.Auto
