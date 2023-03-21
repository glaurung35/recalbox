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
        readonly
        show-value
        size="120px"
        track-color="light-blue"
        v-model="value"
      >
        {{ value + (type === 'percent' ? '%' : '') }}
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
  title: { type: String, default: '' },
  value: { type: Number, default: 0 },
  type: { type: String, default: 'number' },
});

const {
  title, value, type,
} = toRefs(props);
</script>
