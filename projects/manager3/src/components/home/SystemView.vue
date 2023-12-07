<template>
  <div class="header">
    <div class="title">
      <q-icon name="icon-emustation" size="md"/>
      {{ $t('home.system.title') }}
    </div>
    <div class="controls">
      <q-btn flat rounded square icon="mdi-restart" text-color="warning" @click="serverStore.esRestart()">
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.system.es.restart') }}
        </q-tooltip>
      </q-btn>
      <q-btn
        flat
        rounded
        square
        icon="mdi-play"
        :text-color="!serverStore.available ? 'positive' : 'secondary'"
        @click="serverStore.esStart()"
        :disable="serverStore.available"
      >
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.system.es.start') }}
        </q-tooltip>
      </q-btn>
      <q-btn
        flat
        rounded
        square
        icon="mdi-stop"
        :text-color="serverStore.available ? 'negative' : 'secondary'"
        @click="serverStore.esStop()"
        :disable="!serverStore.available"
      >
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          {{ $t('home.system.es.stop') }}
        </q-tooltip>
      </q-btn>
    </div>
  </div>
  <div class="informations">
    <div class="lines" v-if="currentState.currentSystem">
      <div
        v-for="color in currentState.currentSystem.metaData.colors"
        class="line"
        :key="color"
        :style="{ backgroundColor: color }"
      ></div>
    </div>
    <q-img
      v-if="currentState.currentSystem"
      class="logo"
      :src="currentState.currentSystem?.logoPath"
      spinner-color="white"
      @click="redirect"
      fit="contain"
    />
    <transition
      appear
      enter-active-class="animated flipInX"
      v-if="currentState.currentSystem"
    >
      <q-list class="meta-list">
        <q-item v-if="currentState.currentSystem.metaData.system.manufacturer">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-factory"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.manufacturer') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.manufacturer}}</q-item-label>
          </q-item-section>
        </q-item>
        <q-item v-if="currentState.currentSystem.metaData.system.yearOfRelease">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-calendar-month-outline"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.yearOfRelease') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.yearOfRelease}}</q-item-label>
          </q-item-section>
        </q-item>
        <q-item v-if="currentState.currentSystem.metaData.system.cpu">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-cpu-32-bit"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.cpu') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.cpu}}</q-item-label>
          </q-item-section>
        </q-item>
        <q-item v-if="currentState.currentSystem.metaData.system.ram">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-memory"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.ram') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.ram}}</q-item-label>
          </q-item-section>
        </q-item>
        <q-item v-if="currentState.currentSystem.metaData.system.gpu">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-expansion-card-variant"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.gpu') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.gpu}}</q-item-label>
          </q-item-section>
        </q-item>
        <q-item v-if="currentState.currentSystem.metaData.system.soundChip">
          <q-item-section top avatar>
            <q-avatar
              color="white"
              square
              rounded
              icon="mdi-toslink"
            />
          </q-item-section>
          <q-item-section>
            <q-item-label>{{ $t('home.system.soundChip') }}</q-item-label>
            <q-item-label caption>{{currentState.currentSystem.metaData.system.soundChip}}</q-item-label>
          </q-item-section>
        </q-item>
      </q-list>
    </transition>
  </div>
</template>

<script lang="ts" setup>
import { useRouter } from 'vue-router';
import { useEmulationstationStore } from 'stores/configuration/emulationstation';
import { useServerStore } from 'stores/server';

const router = useRouter();

const serverStore = useServerStore();
const emulationStationStore = useEmulationstationStore();

const { currentState } = emulationStationStore;

serverStore.available = true;

function redirect() {
  if (currentState.currentSystem?.systemId === 'imageviewer') {
    router.push(
      { name: 'screenshots' },
    );
  } else {
    router.push(
      { name: 'systems-system', params: { system: currentState.currentSystem?.systemId } },
    );
  }
}

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
    margin-left: 1em

    .q-icon
      margin-right: 0.2em

  .controls
    display: flex
    justify-content: flex-end
    gap: 0.5em

.informations
  display: flex
  flex-direction: column
  padding: 0 1em

  .lines
    position: absolute
    display: flex
    flex-direction: row
    top: 44px
    left: 15px
    width: 6%
    height: 100%
    opacity: 0.3

    .line
      width: 20%
      min-height: 100%

  .meta-list
    border-color: white
    border-radius: 5px

  .logo
    cursor: pointer
    width: 50%
    margin: 0 auto 2em auto
    filter: saturate(50%)
    opacity: 0.4
    transition: opacity 0.3s
    height: 80px

    &:hover
      filter: initial
      opacity: 1
</style>
