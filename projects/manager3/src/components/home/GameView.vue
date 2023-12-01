<template>
  <div class="header">
    <div class="title">
      <q-icon name="icon-emustation" size="md"/>
      {{ $t('home.game.title') }}
    </div>
    <div class="controls">
      <q-btn
        flat
        rounded
        square
        icon="mdi-stop"
        text-color="negative"
        disable
      >
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.game.stop') }}
        </q-tooltip>
      </q-btn>
    </div>
  </div>
  <div class="informations">
    <div>{{ currentState.currentRom?.name }}</div>
    <div>{{ currentState.currentRom?.imagePath }}</div>
    <div class="screen">
      <img src="../../assets/game-exemple.png" alt="Hokuto No Ken">
    </div>
  </div>
</template>

<script lang="ts" setup>
import { useEmulationstationStore } from 'stores/configuration/emulationstation';

const emulationStationStore = useEmulationstationStore();
const { currentState } = emulationStationStore;
</script>

<style lang="sass" scoped>
.header
  display: flex
  justify-content: space-between
  gap: 1em
  margin-bottom: 1em
  padding: 0.2em
  border-bottom: 2px solid white

  .title
    display: flex
    align-items: center
    text-transform: uppercase
    font-weight: 400
    color: $accent
    min-height: 36px
    margin-left: 1em

    .q-icon
      margin-right: 0.2em

.informations
  display: flex
  justify-content: center
  padding: 0 1em

  .screen
    @keyframes flicker
      $steps:20
      @for $i from 0 through $steps
        #{percentage($i * ( 1 / $steps ))}
          opacity: random()

    $screen-background: #121010

    position: relative
    width: 70%
    border-bottom-left-radius: 100% 4%
    border-bottom-right-radius: 100% 4%
    border-top-left-radius: 100% 4%
    border-top-right-radius: 100% 4%

    // flicker
    &::after
      content: ' '
      display: block
      position: absolute
      top: 0
      left: 0
      bottom: 0
      right: 0
      background: transparentize($screen-background,0.9)
      opacity: 0
      z-index: 2
      pointer-events: none
      animation: flicker 0.15s infinite
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%

    // scanlines
    &::before
      content: ' '
      display: block
      position: absolute
      top: 0
      left: 0
      bottom: 0
      right: 0
      background: linear-gradient(transparentize($screen-background,1) 50%, transparentize(darken($screen-background,10),0.75) 50%), linear-gradient(90deg,transparentize(#ff0000,0.94), transparentize(#00ff00,0.98),transparentize(#0000ff,0.94))
      z-index: 2
      background-size: 100% 2px, 3px 100%
      pointer-events: none
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%

    img
      display: block
      width: 100%
      border-bottom-left-radius: 100% 4%
      border-bottom-right-radius: 100% 4%
      border-top-left-radius: 100% 4%
      border-top-right-radius: 100% 4%
      opacity: 0.8
</style>
