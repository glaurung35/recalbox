<!--
@author Nicolas TESSIER aka Asthonishia
-->
<template>
  <div class="row">
    <div class="col col-xs-12 col-sm-12 col-md-12">
      <FormFragmentContainer title="settings.hyperion.settings.title">
        <template v-slot:content>
          <WrappedToggle
            label="settings.hyperion.settings.enableHyperion"
            :getter="hyperion.enabled"
            :setter="hyperionStore.post"
            apiKey="enabled"
            v-if="hyperion.enabled"
          />
        </template>
      </FormFragmentContainer>
    </div>
  </div>
</template>

<script lang="ts" setup>
import WrappedToggle from 'components/global/WrappedToggle.vue';
import { useHyperionStore } from 'stores/configuration/hyperion';
import { storeToRefs } from 'pinia';
import FormFragmentContainer from '../global/FormFragmentContainer.vue';

const hyperionStore = useHyperionStore();
hyperionStore.fetch();
const { hyperion } = storeToRefs(hyperionStore);
</script>
