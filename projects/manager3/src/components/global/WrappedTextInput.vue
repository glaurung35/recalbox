<template>
  <q-input
    :label="$t(this.label)"
    dense
    standout="bg-primary text-white"
    v-model="text"
    v-bind="$attrs"
    :class="this.customClass ? this.customClass : 'q-mb-md'"
    :type="this.isPwd && this.password ? 'password' : 'text'"
  >
    <template v-slot:append v-if="this.password">
      <q-icon
        :name="isPwd ? 'mdi-eye-off' : 'mdi-eye'"
        class="cursor-pointer"
        @click="isPwd = !isPwd"
      />
    </template>
  </q-input>
</template>

<script>
  export default {
    name: 'WrappedTextInput',
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
      },
      password: {
        type: Boolean
      }
    },
    computed: {
      text: {
        get: function() {
          return this.$store.getters[this.getter] ? this.$store.getters[this.getter].value : ''
        },
        set: function(value) {
          clearTimeout(this.timeout)

          this.timeout = setTimeout(() => {
            this.$store.dispatch(this.setter, {[this.apiKey]: value})
          }, 700)
        }
      }
    },
    data() {
      return {
        timeout: 0,
        isPwd: true,
      }
    }
  }
</script>

<style lang="sass">

</style>
