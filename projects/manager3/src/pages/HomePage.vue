<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <q-page class="background home">
    <div class="header">
      <div class="background-container">
        <img src="../assets/recalbox.svg" alt="Recalbox" />
      </div>
    </div>
    <div class="row home-stats">
      <div class="vertical-middle text-center preview-title self-center preview-title">
        {{ $t('home.preview.title') }}
        <span class="arrow-right"><q-icon name="mdi-chevron-right"/></span>
        <span class="arrow-bottom"><q-icon name="mdi-chevron-down"/></span>
      </div>
      <q-separator vertical/>
      <q-separator/>

      <!-- PC version -->
      <SystemStat
        :key="stat.key"
        v-bind="stat"
        v-for="stat in stats"
      />

      <!-- Mobile version -->
      <div class="col-12 linear-progress" style="padding: 16px">
        <div class="linear-progress-title text-accent">{{ $t('home.preview.systems') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.4" color="secondary" size="15px"/>
        </div>
        <div class="linear-progress-title text-accent">{{ $t('home.preview.roms') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.4" color="secondary" size="15px"/>
        </div>
        <div class="linear-progress-title text-accent">{{ $t('home.preview.sharePercent') }}</div>
        <div class="linear-progress-bar">
          <q-linear-progress :value="0.54" color="secondary" size="15px"/>
        </div>
      </div>
    </div>

    <div class="row flex items-stretch home-emustation">
      <div class="col col-md-4 machines-list">
        Nombre de jeux par machines
      </div>
      <div class="col col-md-4 system">
        <SystemView/>
      </div>
      <div class="col col-md-4 game">
        <GameView/>
      </div>
    </div>
  </q-page>
</template>

<script lang="ts" setup>
import { useMonitoringStore } from 'src/stores/monitoring';
import { computed } from 'vue';
import SystemStat from 'components/home/SystemStat.vue';
import { useSystemsStore } from 'stores/systems';
import SystemView from 'components/home/SystemView.vue';
import GameView from 'components/home/GameView.vue';

const { getSystemsListCount } = useSystemsStore();
useSystemsStore().fetch();

const { getSharePercent } = useMonitoringStore();
useMonitoringStore().fetch();

const stats = computed<object[]>(() => [
  {
    key: 1,
    title: 'home.preview.systems',
    value: getSystemsListCount(),
    percent: 100,
  },
  {
    key: 2,
    title: 'home.preview.roms',
    value: 70543,
    percent: 100,
  },
  {
    key: 3,
    title: 'home.preview.sharePercent',
    value: getSharePercent(),
    percent: getSharePercent(),
  },
]);
</script>

<style lang="sass" scoped>
.home
  height: 100%

  &:before
    content: "\F056E"

  .header
    height: 5em
    background-image: linear-gradient(to right, $primary, $accent)
    -webkit-box-shadow: inset 0 0 15px 4px rgba(0,0,0,0.29)
    box-shadow: inset 0 0 15px 4px rgba(0,0,0,0.29)

    .background-container
      height: 100%
      background-image: url(../assets/controller-background.png)
      position: relative

      @keyframes logoslidein
        from
          right: -38rem
        to
          right: 1em

      img
        max-width: 310px
        position: absolute
        right: 1em
        bottom: 0
        animation-duration: .5s
        animation-name: logoslidein

  .home-stats
    background: white
    min-height: 212px
    position: relative

    .preview-title
      text-transform: uppercase
      font-weight: 100
      font-size: x-large
      color: $primary

    .stat
      width: calc((85% / 4) - (1px / 4))

    .home-stat-title
      text-transform: uppercase

    .col-2
      width: calc(16.6667% - (1px / 4))

    .linear-progress-title
      text-transform: uppercase

    .linear-progress-bar
      margin-bottom: .5em

    @media(max-width: 1023px)
      .preview-title
        width: 100%

      .arrow-right,
      .q-separator--vertical
        display: none

    @media(max-width: 900px)
      .stat
        display: none

    @media(min-width: 901px)
      .linear-progress
        display: none

    @media(min-width: 1024px)
      .preview-title
        width: 15%

      .arrow-bottom,
      .q-separator--horizontal
        display: none

  .q-separator--vertical
    background: $rc-light-grey
    width: 2px
    min-width: 2px
    margin-bottom: 0

  .q-separator--horizontal
    background: $rc-light-grey
    width: 100%
    height: 2px

  .home-emustation
    height: calc(100vh - 212px - 5em)

    .machines-list,
    .system
      border-right: 2px solid white

    .machines-list,
    .system,
    .game
      position: relative
      display: flex
      flex-direction: column

    .machines-list,
    .system
      background: $rc-light-grey

    .system
      &:after
        display: block
        content: ''
        border-top: 2px solid white
        border-right: 2px solid white
        height: 50px
        width: 50px
        transform: rotate(45deg) translateY(-50%)
        position: absolute
        right: -8px
        background: $rc-light-grey
        margin-top: calc(50vh - 131px)
        z-index: 1
</style>
