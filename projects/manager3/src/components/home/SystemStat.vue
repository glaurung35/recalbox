<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <transition
    appear
    enter-active-class="animated flipInX"
  >
    <div class="col q-pa-md text-center stat">
      <q-knob
        :step="10"
        :thickness="0.3"
        class="text-accent q-ma-md"
        color="secondary"
        font-size="1rem"
        show-value
        size="120px"
        track-color="light-blue"
        v-model="percent"
        readonly
      >
        {{ value + (value <= 100 ? '%' : '') }}
      </q-knob>
      <div class="text-subtitle1 text-accent home-stat-title">
        {{ $t(title) }}
      </div>
    </div>
  </transition>
</template>

<script lang="ts" setup>
import { toRefs } from 'vue';

const props = defineProps({
  title: { type: String },
  value: { type: Number, default: 0 },
  percent: { type: Number, default: 0 },
});

const {
  title, value, percent,
} = toRefs(props);
</script>
