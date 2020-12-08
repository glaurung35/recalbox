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

<script>
  export default {
    name: 'Stat',
    props: {
      title: {
        type: String,
        required: true,
      },
      value: {
        type: Number,
        required: true,
      },
      type: {
        type: String,
        default: 'number',
      },
    },
  }
</script>

<style lang="sass">

</style>
