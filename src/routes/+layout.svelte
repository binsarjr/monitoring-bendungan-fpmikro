<script lang="ts">
	import DatabaseReady from '$lib/components/DatabaseReady.svelte';
	import KetinggianAirStoreFunction from '$lib/components/firebase-stores/KetinggianAirStoreFunction.svelte';
	import '../app.css';
</script>

<slot />

<DatabaseReady>
	<KetinggianAirStoreFunction />
</DatabaseReady>
