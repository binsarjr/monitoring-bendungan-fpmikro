import { get, writable } from 'svelte/store'

export const ketinggianAir = writable(0);

export const status = writable<'Belum Terdeteksi' | 'aman' | 'siaga' | 'bahaya' | 'bencana'>(
	'Belum Terdeteksi'
);

export const lastUpdate = writable('');

export const dataLogicReady = writable(false);
export const maxAman = writable(0);
export const minSiaga = writable(0);
export const maxSiaga = writable(0);
export const minBahaya = writable(0);
export const maxBahaya = writable(0);
export const minBencana = writable(0);

export const isAman = (ketinggianAir: number) => ketinggianAir < get(maxAman);
export const isSiaga = (ketinggianAir: number) =>
	ketinggianAir >= get(minSiaga) && ketinggianAir < get(maxSiaga);
export const isBahaya = (ketinggianAir: number) =>
	ketinggianAir >= get(minBahaya) && ketinggianAir < get(maxBahaya);

export const isBencana = (ketinggianAir: number) => ketinggianAir >= get(minBencana);
