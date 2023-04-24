<template>
  <div class="header">
    <div class="title">Système en cours</div>
    <div class="controls">
      <q-btn flat rounded square icon="mdi-restart" text-color="warning">
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          Restart ES
        </q-tooltip>
      </q-btn>
      <q-btn flat rounded square icon="mdi-power" text-color="negative">
        <q-tooltip
          class="bg-primary"
          :offset="[10, 10]"
          content-class="bg-primary"
          content-style="font-size: 16px"
        >
          Shutdown ES
        </q-tooltip>
      </q-btn>
    </div>
  </div>
  <div class="informations">
    <q-img
      class="logo"
      src="http://192.168.1.30:20666/api/systems/atomiswave/resource/eu/svg/logo"
      spinner-color="white"
      alt="Sammy Atomiswave"
      @click="() => $router.push(
        { name: 'systems-system', params: { system: 'atomiswave' }}
      )"
    />
    <q-list class="meta-list">
      <q-item
        v-for="system in fakeSystemMeta"
        v-bind="system"
        :key="system.label"
      >
        <q-item-section top avatar>
          <q-avatar
            color="white"
            square
            rounded
            :icon="system.icon"
          />
        </q-item-section>
        <q-item-section>
          <q-item-label>{{system.label}}</q-item-label>
          <q-item-label caption>{{system.value}}</q-item-label>
        </q-item-section>
      </q-item>
    </q-list>
  </div>
</template>

<script lang="ts" setup>
const fakeSystemMeta = [
  { label: 'Manufacturer', value: 'Sammy', icon: 'mdi-factory' },
  { label: 'Year of Release', value: '2003', icon: 'mdi-calendar-month-outline' },
  { label: 'CPU', value: 'Hitachi SH-4 32-bit RISC @ 200 MHz (360 MIPS / 1,4 GFLOPS)', icon: 'mdi-cpu-32-bit' },
  { label: 'RAM', value: '32 Mo', icon: 'mdi-memory' },
  { label: 'GPU', value: 'PowerVR 2 (PVR2DC)', icon: 'mdi-expansion-card-variant' },
  { label: 'Sound chip', value: 'Yamaha ARM7 AICA @ 45 MHz (CPU interne 32-bit RISC, 64 canaux ADPCM)', icon: 'mdi-toslink' },
];
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
    font-weight: 300
    color: $accent
    margin-left: 1em

  .controls
    display: flex
    justify-content: flex-end
    gap: 0.5em

.informations
  display: flex
  flex-direction: column
  padding: 0 1em

  .meta-list
    border-color: white
    border-radius: 5px

  .logo
    cursor: pointer
    width: 50%
    margin: 0 auto 2em auto
    filter: saturate(0)
    opacity: 0.4

    &:hover
      filter: initial
      opacity: 1
</style>
