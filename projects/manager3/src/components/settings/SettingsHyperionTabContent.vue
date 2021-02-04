<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.hyperion.options.title">
        <template v-slot:content>
          <div class="col col-xs-12 col-sm-12 col-md-12">
            <WrappedToggle
              label="settings.hyperion.options.enableHyperion"
              getter="hyperion/enabled"
              setter="hyperion/post"
              apiKey="enabled"
            />
          </div>
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script>
import WrappedToggle from 'components/global/WrappedToggle'
import FormFragmentContainer from '../global/FormFragmentContainer'

export default {
  name: 'SettingsHyperionTabContent',
  components: {
    WrappedToggle,
    FormFragmentContainer,
  },
  created() {
    this.$store.dispatch('hyperion/get')
  },
  data() {
    return {
      enableKodi: true,
    }
  }
}
</script>

<style lang="sass" scoped>

</style>
