<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <transition
    appear
    enter-active-class="animated flipInX"
  >
    <div class="col container percent">
      <div class="badge">
        <q-knob
          :thickness="1"
          class="percent"
          color="secondary"
          font-size="1rem"
          show-value
          size="120px"
          track-color="light-blue"
          v-model="percent"
          readonly
        />
        <div class="title">
          {{ $t(title) }}
        </div>
      </div>
    </div>
  </transition>
</template>

<script lang="ts" setup>
import { toRefs } from 'vue';

const props = defineProps({
  title: { type: String },
  percent: { type: Number, default: 0 },
});

const {
  title, percent,
} = toRefs(props);
</script>

<style lang="sass" scoped>
.container
  display: flex
  justify-content: center
  align-items: center

  .badge
    display: flex
    flex-direction: column
    justify-content: center
    align-items: center
    position: relative

    .percent
      color: white
      text-shadow: -1px 0 $secondary, 0 1px $secondary, 1px 0 $secondary, 0 -1px $secondary

    .title
      text-align: center
      text-transform: uppercase
      color: white
      text-shadow: -1px 0 $secondary, 0 1px $secondary, 1px 0 $secondary, 0 -1px $secondary
      position: absolute
      top: 5rem
      left: 0
      right: 0
</style>
