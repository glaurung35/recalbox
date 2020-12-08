<template>
  <q-toggle
    :label="$t(this.label)"
    :class="this.customClass ? this.customClass : 'q-mb-md'"
    v-model="enabled"
    v-bind="$attrs"
  />
</template>

<script>
  export default {
    name: 'WrappedToggle',
    props: {
      label: {
        type: String,
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
      enabled: {
        get: function() {
          return this.$store.getters[this.getter] ? this.$store.getters[this.getter].value : false
        },
        set: function(value) {
          this.$store.dispatch(this.setter, {[this.apiKey]: value})
        }
      }
    }
  }
</script>

<style lang="sass">

</style>
