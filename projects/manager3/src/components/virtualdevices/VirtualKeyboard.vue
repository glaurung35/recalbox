<!--
@author Sesram
-->
<template>
  <div>
    <div class="col col-xs-6 col-sm-12 col-md-12">
      <q-select
        standout="bg-primary text-white"
        v-model="keyboardDisplay"
        :options="keyboardOptions"
        label="Layout"
        dense
      />
    </div>
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <div class="keyboardContainer">
        <div class="simple-keyboard-main"></div>

        <div class="controlArrows">
          <div class="simple-keyboard-control"></div>
          <div class="simple-keyboard-arrows"></div>
        </div>

        <div class="numPad">
          <div class="simple-keyboard-numpad"></div>
          <div class="simple-keyboard-numpadEnd"></div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import Keyboard from 'simple-keyboard';
import 'simple-keyboard/build/css/index.css';

export default {
  name: 'VirtualKeyboard',
  props: {
    keyboardClass: {
      default: 'simple-keyboard',
      type: String,
    },
    input: {
      type: String,
    },
    fullKeyboard: {
      default: true,
      type: Boolean,
    },
    layout: {
      default: 'qwerty',
      type: String,
    },
  },
  data: () => ({
    keyboard: null,
    keyboardControlPad: null,
    keyboardArrows: null,
    keyboardNumPad: null,
    keyboardNumPadEnd: null,
    keyboardOptions: [
      { label: 'QWERTY (english)', value: 'qwerty' },
      { label: 'AZERTY (french)', value: 'azerty' },
    ],
    keyboardDisplay: {
      default: { label: 'QWERTY (english)', value: 'qwerty' },
      type: { label: String, value: String },
    },
  }),
  mounted() {
    const commonKeyboardOptions = {
      onChange: (input) => this.onChange(input),
      onKeyPress: (button) => this.onKeyPress(button),
      onKeyReleased: (button) => this.onKeyReleased(button),
      theme: 'simple-keyboard hg-theme-default hg-layout-default',
      physicalKeyboardHighlight: true,
      syncInstanceInputs: true,
      mergeDisplay: true,
      debug: true,
    };
    this.keyboard = new Keyboard('.simple-keyboard-main', {
      ...commonKeyboardOptions,
      /**
       * Layout by:
       * Sterling Butters (https://github.com/SterlingButters)
       */
      layout: {
        default: [
          '{escape} {f1} {f2} {f3} {f4} {f5} {f6} {f7} {f8} {f9} {f10} {f11} {f12}',
          '{KEY41} {KEY2} {KEY3} {KEY4} {KEY5} {KEY6} {KEY7} {KEY8} {KEY9} {KEY10} {KEY11} {KEY12} {KEY13} {backspace}',
          '{tab} {KEY16} {KEY17} {KEY18} {KEY19} {KEY20} {KEY21} {KEY22} {KEY23} {KEY24} {KEY25} {KEY26} {KEY27}',
          '{capslock} {KEY30} {KEY31} {KEY32} {KEY33} {KEY34} {KEY35} {KEY36} {KEY37} {KEY38} {KEY39} {KEY40} {enter}',
          '{shiftleft} {KEY43} {KEY44} {KEY45} {KEY46} {KEY47} {KEY48} {KEY49} {KEY50} {KEY51} {KEY52} {KEY53} {shiftright}',
          '{ctrlleft} {metaleft} {altleft} {space} {altright} {metaright} {menu} {ctrlright}',
        ],
        shift: [
          '{escape} {f1} {f2} {f3} {f4} {f5} {f6} {f7} {f8} {f9} {f10} {f11} {f12}',
          '{SKEY41} {SKEY2} {SKEY3} {SKEY4} {SKEY5} {SKEY6} {SKEY7} {SKEY8} {SKEY9} {SKEY10} {SKEY11} {SKEY12} {SKEY13} {backspace}',
          '{tab} {SKEY16} {SKEY17} {SKEY18} {SKEY19} {SKEY20} {SKEY21} {SKEY22} {SKEY23} {SKEY24} {SKEY25} {SKEY26} {SKEY27}',
          '{capslock} {SKEY30} {SKEY31} {SKEY32} {SKEY33} {SKEY34} {SKEY35} {SKEY36} {SKEY37} {SKEY38} {SKEY39} {SKEY40} {enter}',
          '{shiftleft} {SKEY43} {SKEY44} {SKEY45} {SKEY46} {SKEY47} {SKEY48} {SKEY49} {SKEY50} {SKEY51} {SKEY52} {SKEY53} {shiftright}',
          '{ctrlleft} {metaleft} {altleft} {space} {altright} {metaright} {menu} {ctrlright}',
        ],
      },
      display: {
        '{KEY2}': '1',
        '{KEY3}': '2',
        '{KEY4}': '3',
        '{KEY5}': '4',
        '{KEY6}': '5',
        '{KEY7}': '6',
        '{KEY8}': '7',
        '{KEY9}': '8',
        '{KEY10}': '9',
        '{KEY11}': '0',
        '{KEY12}': '-',
        '{KEY13}': '=',
        '{KEY16}': 'q',
        '{KEY17}': 'w',
        '{KEY18}': 'e',
        '{KEY19}': 'r',
        '{KEY20}': 't',
        '{KEY21}': 'y',
        '{KEY22}': 'u',
        '{KEY23}': 'i',
        '{KEY24}': 'o',
        '{KEY25}': 'p',
        '{KEY26}': '[',
        '{KEY27}': ']',
        '{KEY30}': 'a',
        '{KEY31}': 's',
        '{KEY32}': 'd',
        '{KEY33}': 'f',
        '{KEY34}': 'g',
        '{KEY35}': 'h',
        '{KEY36}': 'j',
        '{KEY37}': 'k',
        '{KEY38}': 'l',
        '{KEY39}': ';',
        '{KEY40}': '\'',
        '{KEY41}': '`',
        '{KEY43}': '\\',
        '{KEY44}': 'z',
        '{KEY45}': 'x',
        '{KEY46}': 'c',
        '{KEY47}': 'v',
        '{KEY48}': 'b',
        '{KEY49}': 'n',
        '{KEY50}': 'm',
        '{KEY51}': ',',
        '{KEY52}': '.',
        '{KEY53}': '/',
        '{escape}': 'esc ⎋',
        '{tab}': 'tab ⇥',
        '{backspace}': 'backspace ⌫',
        '{enter}': 'enter ↵',
        '{capslock}': 'caps lock ⇪',
        '{shiftleft}': 'shift ⇧',
        '{shiftright}': 'shift ⇧',
        '{ctrlleft}': 'ctrl ⌃',
        '{ctrlright}': 'ctrl ⌃',
        '{altleft}': 'alt ⌥',
        '{altright}': 'alt ⌥',
        '{metaleft}': 'cmd/Win',
        '{metaright}': 'cmd ⌘',
        '{menu}': 'Menu',
        '{SKEY2}': '!',
        '{SKEY3}': '@',
        '{SKEY4}': '#',
        '{SKEY5}': '$',
        '{SKEY6}': '%',
        '{SKEY7}': '^',
        '{SKEY8}': '&',
        '{SKEY9}': '*',
        '{SKEY10}': '(',
        '{SKEY11}': ')',
        '{SKEY12}': '_',
        '{SKEY13}': '+',
        '{SKEY16}': 'Q',
        '{SKEY17}': 'W',
        '{SKEY18}': 'E',
        '{SKEY19}': 'R',
        '{SKEY20}': 'T',
        '{SKEY21}': 'Y',
        '{SKEY22}': 'U',
        '{SKEY23}': 'I',
        '{SKEY24}': 'O',
        '{SKEY25}': 'P',
        '{SKEY26}': '{',
        '{SKEY27}': '}',
        '{SKEY30}': 'A',
        '{SKEY31}': 'S',
        '{SKEY32}': 'D',
        '{SKEY33}': 'F',
        '{SKEY34}': 'G',
        '{SKEY35}': 'H',
        '{SKEY36}': 'J',
        '{SKEY37}': 'K',
        '{SKEY38}': 'L',
        '{SKEY39}': ':',
        '{SKEY40}': '\'',
        '{SKEY41}': '~',
        '{SKEY43}': '|',
        '{SKEY44}': 'Z',
        '{SKEY45}': 'X',
        '{SKEY46}': 'C',
        '{SKEY47}': 'V',
        '{SKEY48}': 'B',
        '{SKEY49}': 'N',
        '{SKEY50}': 'M',
        '{SKEY51}': '<',
        '{SKEY52}': '>',
        '{SKEY53}': '?',

      },
    });
    this.keyboardControlPad = new Keyboard('.simple-keyboard-control', {
      ...commonKeyboardOptions,
      layout: {
        default: [
          '{insert} {home} {pageup}',
          '{delete} {end} {pagedown}',
        ],
      },
    });
    this.keyboardArrows = new Keyboard('.simple-keyboard-arrows', {
      ...commonKeyboardOptions,
      layout: {
        default: ['{arrowup}', '{arrowleft} {arrowdown} {arrowright}'],
      },
    });
    this.keyboardNumPad = new Keyboard('.simple-keyboard-numpad', {
      ...commonKeyboardOptions,
      layout: {
        default: [
          '{numlock} {numpaddivide} {numpadmultiply}',
          '{numpad7} {numpad8} {numpad9}',
          '{numpad4} {numpad5} {numpad6}',
          '{numpad1} {numpad2} {numpad3}',
          '{numpad0} {numpaddecimal}',
        ],
      },
    });
    this.keyboardNumPadEnd = new Keyboard('.simple-keyboard-numpadEnd', {
      ...commonKeyboardOptions,
      layout: {
        default: ['{numpadsubtract}', '{numpadadd}', '{numpadenter}'],
      },
    });
  },
  methods: {
    onChange(input) {
      this.$emit('onChange', input);
    },
    onKeyPress(button) {
      this.$emit('onKeyPress', button);

      /**
       * If you want to handle the shift and caps lock buttons
       */
      if (
        button === '{shift}'
        || button === '{shiftleft}'
        || button === '{shiftright}'
        || button === '{capslock}'
      ) { this.handleShift(); }
    },
    onKeyReleased(button) {
      this.$emit('onKeyReleased', button);

      /**
       * If you want to handle the shift and caps lock buttons
       */
      if (
        button === '{shift}'
        || button === '{shiftleft}'
        || button === '{shiftright}'
      ) { this.handleShift(); }
    },
    handleShift() {
      const currentLayout = this.keyboard.options.layoutName;
      const shiftToggle = currentLayout === 'default' ? 'shift' : 'default';

      this.keyboard.setOptions({
        layoutName: shiftToggle,
      });
    },
  },
  watch: {
    input(input) {
      this.keyboard.setInput(input);
    },
  },
};
</script>

<!-- Add 'scoped' attribute to limit CSS to this component only -->
<style scoped>
  input {
    width: 100%;
    height: 100px;
    padding: 20px;
    font-size: 20px;
    border: none;
    box-sizing: border-box;
  }

  .simple-keyboard {
    max-width: 700px;
  }
</style>
