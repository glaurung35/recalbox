<template>
  <q-select
    :label="$t(this.label)"
    :options="this.options"
    :class="this.customClass ? this.customClass : 'q-mb-md'"
    dense
    standout="bg-primary text-white"
    v-model="selectedValue"
    v-bind="$attrs"
    :map-options="true"
    transition-show="flip-up"
    transition-hide="flip-down"
  />
</template>

<script>
  export default {
    name: 'WrappedSelect',
    props: {
      label: {
        type: String,
        required: true
      },
      options: {
        type: Array,
        required: true
      },
      getter: {
        type: String,
        required: true
      },
      setter: {
        type: String,
        required: true
      },
      apiKey: {
        type: String,
        required: true
      },
      customClass: {
        type: String
      }
    },
    computed: {
      selectedValue: {
        get: function() {
          return this.$store.getters[this.getter] ? this.$store.getters[this.getter].value : ''
        },
        set: function(selected) {
          this.$store.dispatch(this.setter, {[this.apiKey]: selected.value})
        }
      }
    }
  }
</script>

<style lang="sass">

</style>
