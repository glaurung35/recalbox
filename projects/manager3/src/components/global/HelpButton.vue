<template>
  <q-btn
    :color="warning ? 'negative' : 'light-blue'"
    @click="() => helpOpen = true"
    flat
    :icon="warning ? 'mdi-alert' : 'mdi-help-circle-outline'"
    round
    size="md"
    v-if="help"
    :class="toggle ? 'help-button' : ''"
  >
    <q-tooltip
      :offset="[10, 10]"
      content-class="bg-primary"
      content-style="font-size: 16px"
    >
      {{ $t('general.helpButton.tooltip') }}
    </q-tooltip>
  </q-btn>
  <q-dialog transition-hide="flip-up" transition-show="flip-down" v-model="helpOpen">
    <q-card :class="className + ' background help-dialog'">
      <!-- eslint-disable-next-line vue/no-v-text-v-html-on-component -->
      <q-card-section class="text-justify" style="white-space: pre-line;" v-html="help"/>
    </q-card>
  </q-dialog>
</template>

<script lang="ts" setup>
import { ref, toRefs } from 'vue';

const helpOpen = ref<boolean>(false);

const props = defineProps({
  help: { type: String, required: true, default: '' },
  warning: { type: Boolean },
  toggle: { type: Boolean },
});

const { help, warning, toggle } = toRefs(props);

const className = `bg-primary text-white ${warning.value ? 'help-dialog-card-warning' : 'help-dialog-card'}`;
</script>

<style lang="sass">
@keyframes helpDialogSlidein
  from
    right: -10rem
    bottom: -10rem

  to
    right: -30px
    bottom: 0

.help-dialog
  overflow: hidden!important
  &:before
    font-size: 14em
    bottom: 0
    right: -30px
    color: $secondary
    opacity: 0.3
    animation-duration: .5s
    animation-name: helpDialogSlidein

.help-button
  margin-bottom: 1em

.help-dialog-card
  border-left: 6px solid $accent

  &:before
    content: "\F0625"

.help-dialog-card-warning
  border-left: 6px solid $negative

  &:before
    content: "\F0026"
</style>
